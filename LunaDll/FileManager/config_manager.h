#pragma once
#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <json/json.hpp>
#include "../Misc/ResourceFileMapper.h"

extern std::string g_ApplicationPath;

class ConfigPackMiniManager
{
public:
    typedef std::vector<std::string> StringList;
    ConfigPackMiniManager();
    ~ConfigPackMiniManager() {}

    enum EntryType
    {
        BLOCKS = 1,
        BGO,
        NPC,
        X_SECTIONS,
        X_LEVELFILE
    };

    struct ConfigEntry
    {
        size_t id;
        ResourceFileInfo ini_fileinfo[3];
        ResourceFileInfo extra_settings_fileinfo;
        std::string extra_settings_filename;
        nlohmann::json default_extra_settings;
    };

    struct ConfigStore
    {
        ResourceFileMap setup_files;
        ResourceFileMap extra_settings_files;
        std::string extra_settings_root;
        std::string setup_root;
        ResourceFileInfo default_global_extra_settings_fileinfo;
        nlohmann::json default_global_extra_settings;
        std::vector<ConfigEntry> data;
        void clear()
        {
            setup_files.clear();
            extra_settings_files.clear();
            extra_settings_root.clear();
            setup_root.clear();
            default_global_extra_settings_fileinfo.done = false;
            default_global_extra_settings.clear();
            data.clear();
        }
    };

    /**
     * @brief Initialize config pack's directory tree
     * @param config_dir path to config pack's root directory
     */
    void loadConfigPack(const std::string &config_dir);

    void loadStore(EntryType type,
                   ConfigStore &dst,
                   const std::string &file,
                   const std::string &hive_head,
                   const std::string &item_head);

    void setEpisodePath(const std::string &episode_path);
    void setCustomPath(const std::string &custom_path);

    ResourceFileInfo getGlobalExtraSettingsFile(EntryType type);

    std::string mergeGlobalExtraSettings(EntryType type,
                                         const std::string &input,
                                         bool beautify = false);
    std::string mergeExtraSettings(EntryType type,
                                   size_t id,
                                   const std::string &input,
                                   bool beautify = false);

    ResourceFileInfo findFile(const std::string &filename, const ResourceFileMap &root_files);
    ResourceFileInfo findFileInSubfolder(const std::string &filename, const std::string &root_path);

    void loadExtraSettings(nlohmann::json &dst, const std::string &path);

private:
    ResourceFileMap         m_cp_files;
    ResourceFileMap         m_episode_files;
    ResourceFileMap         m_custom_files;

    ConfigStore             m_blocks;
    ConfigStore             m_bgo;
    ConfigStore             m_npc;

    ResourceFileInfo        m_sections_ex_fileinfo;
    ResourceFileInfo        m_level_ex_fileinfo;

    nlohmann::json          m_sections_ex;
    nlohmann::json          m_level_ex;

    bool                    m_is_using;
    std::string             m_cp_root_path;
    std::string             m_episode_path;
    std::string             m_custom_path;
};

#endif // CONFIGMANAGER_H
