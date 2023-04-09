#ifndef _RUNNING_STAT_H_
#define _RUNNING_STAT_H_
#include <cmath>
#include <string>
#include <sstream>

// Based upon Knuth TAOCP vol 2, 3rd edition, page 232
//        and http://www.johndcook.com/blog/standard_deviation/

class RunningStat
{
private:
    int m_count;
    double m_mean, m_sum;
    double m_min, m_max;

public:
    RunningStat() : m_count(0) {}

    void Clear()
    {
        m_count = 0;
    }

    void Push(double x)
    {
        m_count++;

        if (m_count == 1)
        {
            m_mean = x;
            m_sum = 0.0;
            m_min = x;
            m_max = x;
        }
        else
        {
            double diffFromLastMean = x - m_mean;
            m_mean += diffFromLastMean / m_count;
            m_sum += diffFromLastMean * (x - m_mean);
            if (x < m_min) m_min = x;
            if (x > m_max) m_max = x;
        }
    }

    int Count() const
    {
        return m_count;
    }

    double Mean() const
    {
        return (m_count > 0) ? m_mean : 0.0;
    }

    double Var() const
    {
        return ((m_count > 1) ? m_sum / (m_count - 1) : 0.0);
    }

    double StdDev() const
    {
        return std::sqrt(Var());
    }

    double Min() const
    {
        return (m_count > 0) ? m_min : 0.0;
    }

    double Max() const
    {
        return (m_count > 0) ? m_max : 0.0;
    }

    std::string Str() const
    {
        std::ostringstream stream;
        stream.precision(3);
        stream.setf(std::ios::fixed, std::ios::floatfield);
        stream << Mean() << " (" << StdDev() << ", " << Min() << "-" << Max() << ")";
        return stream.str();
    }
};

#endif
