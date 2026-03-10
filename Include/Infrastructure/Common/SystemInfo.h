#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
#include <string>

typedef enum EnumWindowsVersion
{
    Windows,
    Windows32s,
    Windows95,
    Windows95OSR2,
    Windows98,
    Windows98SE,
    WindowsMillennium,
    WindowsNT351,
    WindowsNT40,
    WindowsNT40Server,
    Windows2000,
    WindowsXP,
    WindowsXPProfessionalx64,
    WindowsHomeServer,
    WindowsServer2003,
    WindowsServer2003R2,
    WindowsVista,
    WindowsServer2008,
    Windows7,
    WindowsServer2008R2,
    Windows8,
    WindowsServer2012,
    Windows81,
    WindowsServer2012R2,
    Windows10,
    WindowsServer2016,
} WindowsVersion;

typedef enum EnumWindowsEdition
{
    EditionUnknown,

    Workstation,
    Server,
    AdvancedServer,
    Home,

    Ultimate,
    HomeBasic,
    HomePremium,
    Enterprise,
    HomeBasic_N,
    Business,
    StandardServer,
    DatacenterServer,
    SmallBusinessServer,
    EnterpriseServer,
    Starter,
    DatacenterServerCore,
    StandardServerCore,
    EnterpriseServerCore,
    EnterpriseServerIA64,
    Business_N,
    WebServer,
    ClusterServer,
    HomeServer,
    StorageExpressServer,
    StorageStandardServer,
    StorageWorkgroupServer,
    StorageEnterpriseServer,
    ServerForSmallBusiness,
    SmallBusinessServerPremium,
    HomePremium_N,
    Enterprise_N,
    Ultimate_N,
    WebServerCore,
    MediumBusinessServerManagement,
    MediumBusinessServerSecurity,
    MediumBusinessServerMessaging,
    ServerFoundation,
    HomePremiumServer,
    ServerForSmallBusiness_V,
    StandardServer_V,
    DatacenterServer_V,
    EnterpriseServer_V,
    DatacenterServerCore_V,
    StandardServerCore_V,
    EnterpriseServerCore_V,
    HyperV,
    StorageExpressServerCore,
    StorageStandardServerCore,
    StorageWorkgroupServerCore,
    StorageEnterpriseServerCore,
    Starter_N,
    Professional,
    Professional_N,
    SBSolutionServer,
    ServerForSBSolution,
    StandardServerSolutions,
    StandardServerSolutionsCore,
    SBSolutionServer_EM,
    ServerForSBSolution_EM,
    SolutionEmbeddedServer,
    SolutionEmbeddedServerCore,
    SmallBusinessServerPremiumCore,
    EssentialBusinessServerMGMT,
    EssentialBusinessServerADDL,
    EssentialBusinessServerMGMTSVC,
    EssentialBusinessServerADDLSVC,
    ClusterServer_V,
    Embedded,
    Starter_E,
    HomeBasic_E,
    HomePremium_E,
    Professional_E,
    Enterprise_E,
    Ultimate_E
} WindowsEdition;

class SystemInfo  
{
    WindowsVersion   m_nWinVersion;
    WindowsEdition   m_nWinEdition;
    TCHAR            m_szServicePack[128];
    OSVERSIONINFOEX  m_osvi;
    SYSTEM_INFO      m_SysInfo;
    BOOL             m_bOsVersionInfoEx;
    TCHAR            m_szVersion[128];
    bool             m_bCompatibilityMode;

private:
    void DetectWindowsVersion();
    void DetectWindowsEdition();
    void DetectWindowsServicePack();
    DWORD DetectProductInfo();

public:
    SystemInfo();
    virtual ~SystemInfo();

    std::string GetWindowsVersionString() const;
    std::string GetServicePackString() const;
    bool IsCompatibilityMode() const;
    WindowsVersion GetWindowsVersion() const;       // returns the windows version
    WindowsEdition GetWindowsEdition() const;       // returns the windows edition
    bool IsNTPlatform() const;                      // true if NT platform
    bool IsWindowsPlatform() const;                 // true is Windows platform
    bool IsWin32sPlatform() const;                  // true is Win32s platform
    DWORD GetMajorVersion() const;                  // returns major version
    DWORD GetMinorVersion() const;                  // returns minor version
    DWORD GetBuildNumber() const;                   // returns build number
    DWORD GetPlatformID() const;                    // returns platform ID
    void GetServicePackInfo(TCHAR* szServicePack) const;// additional information about service pack
    bool Is32bitPlatform() const;                   // true if platform is 32-bit
    bool Is64bitPlatform() const;                   // true if platform is 64-bit
};
