#pragma once

#include "JackCDevService.h"
#include "FilePath.h"

namespace JackC
{
    class JACKC_COMMON_EXPORT FilePathManager
    {
        CANNOT_COPY_OR_ASSIGN(FilePathManager)
    public:
        static FilePathManager* Get();

        virtual ~FilePathManager(void);

        //不在FilePathManager构造时直接初始化，是由于必须保证先
        //FilePathManager::Get()->SetBehaviorConfigFilePath(),再根据BehaviorConfig初始化各类路径
        void Initialize();

        const FilePath& GetResourcePath();

        // 不允许随便设置PublishConfigFile的路径
        const FilePath& GetPublishConfigFilePath();

        const FilePath& GetBehaviorConfigFilePath();
      
        // 必须在外部程序启动时设置相应BehaviorConfig文件路径
        void SetBehaviorConfigFilePath(const std::wstring& behaviorConfigFilePath);

        const FilePath& GetLogsFolderPath();
        void SetLogsFolderPath(const std::wstring& logsFolderPath);

        const FilePath& GetTemplateFolderPath();
        void SetTemplateFolderPath(const std::wstring& templateFolderPath);

        const FilePath& GetGitRevisionFilePath();
        void SetGitRevisionFilePath(const std::wstring& gitRevisionFilePath);
    private:
        FilePathManager();
        void InitAppName();
        void InitPublishConfigFilePath();
        void InitLogsFolderPath();
        void InitTemplateFilePath();
        void InitGitRevisionFilePath();
        void InitResourceFilePath();

    private:
        std::wstring m_appName;
        FilePath m_publishConfigFilePath;
        FilePath m_behaviorConfigFilePath;
        FilePath m_logsFolderPath;
        FilePath m_templateFolderPath;
        FilePath m_gitRevisionFilePath;
        FilePath m_resourceFilePath;

        bool m_isInitialled;
    };
}


