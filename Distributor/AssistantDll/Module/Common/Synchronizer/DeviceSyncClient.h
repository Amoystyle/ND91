#pragma once
#include "Module/Common/Synchronizer/ServiceClientBase.h"

namespace ND91Assistant
{
    class DeviceLink;

    // 主要用来处理<see cref="DeviceSyncType"/>用的
    struct DeviceSyncAnchors
    {
        string _deviceAnchor;
        string _computerAnchor;
        DeviceSyncAnchors()
        {
            _deviceAnchor   = "";
            _computerAnchor = "";
        }
    };

    class DeviceSyncClient : public ServiceClientBase
    {
    public:
        DeviceSyncClient(IosUsbConnection* device);
        ~DeviceSyncClient();

        // 同步类型
        enum DeviceSyncType
        {
            Fast,    // Fast-sync 只需要传送上次同步以来变更过的数据即可
            Slow,    // Slow-sync 需要传送所有数据
            Reset,   // Reset-sync signals that the computer should send all data again.(设备某个DataClass第一次同步的时候是这个类型)
            Unknow
        };

        // 数据的方向
        enum DeviceSyncDirection
        {
            DeviceToComputer,  // 设备到PC
            ComputerToDevice,  // PC到设备
            Unknown
        };

        string GetEmptyParameterString() { return EMPTY_PARAMETER_STRING; }
        string GetRecordEntityNameKey() { return RECORD_ENTITY_NAME_KEY; }
        string GetDataClass() { return _dataClass; }
        DeviceSyncDirection GetDeviceSyncDirection() { return _syncDirection; }
        string GetDeviceAnchor() { return _deviceAnchor; }
        DeviceSyncType GetDeviceSyncType() { return _syncType; }

        // 判断当前设备MobileSync服务是否已经准备接收来自PC的数据了.
        // 要进入这个步骤,需要调用GetAllRecordsFromDevice或者GetChangesFromDevice并进行相应处理
        bool IsReadyToReceiveChangesFromComputer();

        // Summary  : 启动设备上mobilesync 服务,用于后续服务调用
        // Returns  : bool
        // Parameter: string dataClass 指定当前要调用那个dataClass(联系人,行事历,还是书签等)
        // Parameter: DeviceSyncAnchors anchors 同步双方用于确定同步类型用的时间锚
        // Parameter: int computerDataClassVersion 当前pc请求dataClass对应的版本
        // Parameter: int * deviceDataClassVersion 当前设备dataClass对应的版本
        bool StartSync(string dataClass, DeviceSyncAnchors anchors, int computerDataClassVersion,int* deviceDataClassVersion);

        // Summary  : 停止同步
        // Returns  : void
        void StopSync();

        // Summary  : 判断某个DataClass是否禁止和PC同步
        // Returns  : bool
        // Parameter: string dataClass
        // Parameter: bool tryEnableIfDisabled 如果禁止,尝试开启
        bool IsDataClassDisableSync(string dataClass, bool tryEnableIfDisabled);

        // Summary  : 判断某个DataClass是否开启iCloud同步或者其他IMAP 邮件账户同步
        // Returns  : bool
        // Parameter: string dataClass
        bool IsDataClassEnableiCloud(string dataClass);

        // Summary  : 告诉设备PC同步已经完成,一般调用该方法后,设备就会显示最新的数据
        // Returns  : bool
        bool FinishSync();

        // Summary  : 告诉设备,PC想要获取从上次同步以来变更的数据
        // Returns  : bool
        bool GetChangesFromDevice();

        // Summary  : 告诉设备,PC想要获取设备上所有数据
        // Returns  : bool
        bool GetAllRecordsFromDevice();

        // Summary  : 接收方法GetAllRecordsFromDevice或者GetChangesFromDevice调用后,设备实际传送过来的数据
        // Returns  : bool
        // Parameter: map<string, map<string,void*>>& mapEntities 接收到设备传回的数据
        // Parameter: bool& isHaveRecord 是否还有数据
        bool ReceiveChangesEntitiesFromDevice(map< string, map<string,void*> >& mapEntities,bool& isHaveRecord);

        // Summary  : 每接收ReceiveChangesEntitiesFromDevice 一次要调用下本方法,告诉设备,
                        // PC已经处理了设备上次传送过来的数据了,设备可以接着传送下一条数据
        // Returns  : bool
        bool AcknowledgeChangesFromDevice();

        // Summary  : 发送PC上变更过的数据给设备
        // Returns  : bool
        // Parameter: vector<void*> sendInfo
        bool SendProcessChangesToDevice(vector<void*> sendInfo);    // 数组方式
        bool SendProcessChangesToDevice(map<string, void*> entities,bool isHaveRecord,map< string, vector<void*> > actions,vector<void*>& sendInfo);    // 数组方式

        // Summary  : 新增数据给设备后,一般要调用该方法,用于获取设备上重新映射过的数据ID.比如PC 某条记录id是226===>设备映射后是12等...
        // Returns  : bool
        // Parameter: map<string, string>& mapIds 设备传回的ID映射数据
        bool GetRemapIdentifiersFromDevice(map<string, string>& mapIds);

        //vector<string> CreateProcessChangesMessage(map<string,string> entities, bool isHaveRecord, map<string,string> actions, wstring xmlFilePath);

        // Summary  : 清除设备上当前DataClass对应的所有数据
        bool ClearAllRecordsOnDevice();

        static string   EMPTY_PARAMETER_STRING;
        static string   RECORD_ENTITY_NAME_KEY;

    private:
        // Summary  : 开启设备上同步服务
        // Returns  : bool 成功返回TRUE
        bool StartMobileSyncService();

        // Summary  : 告诉设备,PC想要获取那种类型数据(所有数据,还是上次同步后变更的数据)
        // Returns  : bool
        // Parameter: string operation all 或者changes
        bool GetRecordsFromDevice(string operation);

        void CreateProcessChangesMessage(string dataClass,map<string, void*> entities,bool isHaveRecord,map< string, vector<void*> > actions,vector<void*>& sendInfo);

    private:
        int             MSYNC_VERSION_INT1;     // 同步版本参数1
        int             MSYNC_VERSION_INT2;     // 同步版本参数2

        bool            _isServiceRunning;      // 设备上的服务是否正在运行
        DeviceLink*     _dlink;                 // 设备连接

        string          _dataClass;             // 数据类型
        DeviceSyncDirection _syncDirection;     // 数据方向
        string          _deviceAnchor;          // 设备返回的标识
        DeviceSyncType  _syncType;              // 同步类型
    };
}