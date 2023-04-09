#ifndef EXTENDEDQJSONREADER_H
#define EXTENDEDQJSONREADER_H

#include <QFile>
#include <QJsonDocument>
#include <QPair>
#include <utility>

#include "qjsonextractor.h"

class ExtendedQJsonReader
{
    QJsonDocument m_internalDocument;
public:
    //!
    //! \brief ExtendedQJsonReader
    //! \throws QJsonParseException
    //! \throws QJsonFileOpenException
    //! \param file The file to read out
    //!
    ExtendedQJsonReader(QFile &file);
    //!
    //! \brief ExtendedQJsonReader
    //! \throws QJsonParseException
    //! \param data
    //!
    ExtendedQJsonReader(const QByteArray &data);
    ExtendedQJsonReader(const QJsonDocument &document);
    QJsonDocument getQJsonDocument() const;
    QVariantMap toMap() const;
    bool canConvertToMap() const;

    template<typename... Args>
    void extractSafe(const QString& jsonNamespace, Args&&... args){
        if(!m_internalDocument.isObject())
            throw QJsonValidationException("<root>", QJsonValidationException::ValidationError::WrongType);

        QStringList jsonNamespaceSteps;
        QJsonObject currentObject = m_internalDocument.object();
        if(!jsonNamespace.isEmpty()){
            jsonNamespaceSteps = jsonNamespace.split(".");
            for(const QString& nextNamespace : jsonNamespaceSteps){
                currentObject = QJsonExtractor<QJsonObject>()(currentObject, nextNamespace);
            }
        }

        extractNext(currentObject, std::forward<Args>(args)...);
    }

private:

    template<typename T1, typename T2, typename... Args>
    void extractNext(const QJsonObject& dataObj, std::pair<T1, T2*> next, Args&&... args){
        *next.second = QJsonExtractor<T2>()(dataObj, QString(next.first));
        extractNext(dataObj, std::forward<Args>(args)...);
    }

    void extractNext(const QJsonObject&) {}

    void initJson(const QByteArray& data);

};

#endif // EXTENDEDQJSONREADER_H
