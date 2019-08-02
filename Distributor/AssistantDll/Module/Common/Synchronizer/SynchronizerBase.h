#pragma once

namespace ND91Assistant
{
    class DeviceSyncClient;
    class IosUsbConnection;
    class DeviceData;

    // 同步器基类
    class SynchronizerBase
    {
    public:
        SynchronizerBase(const DeviceData* pDevice);
        ~SynchronizerBase();

        // Summary  : 当前是否禁止与PC同步该DataClass,如果是True...
        // Returns  : bool
        // Parameter: bool tryEnableIfDisabled 如果是True,是否要尝试关闭
        bool IsDisableSync(bool tryEnableIfDisabled);

        // Summary  : 判断当前DataClass是否开启iCloud或者其他IMAP 邮件账户同步
                    //(备忘录同步(over air sync)至少要3代(4.0固件)以后的机器才支持账户同步,3gs及其之后,iTouch 3代... iPad)
        // Returns  : bool
        bool IsEnableiCloud();

        // Summary  : 加载当前data-class对应的所有数据
        // Returns  : bool
        bool LoadAllData();

        // Summary  : 删除该dataclass相应的所有数据(这个比调用SaveChanges方法快)
        bool RemoveAllData();

    protected:
        
        // Summary  : 保存修改过的数据(新增,修改,删除)
        // Returns  : bool 成功返回TRUE
        bool SaveChanges();

        // Summary  : 加载当前data-class对应的所有数据
        // Returns  : bool 成功返回TRUE
        // Parameter: bool useFastSync 是否使用快速同步模式(保存数据的时候一般用快速同步模式)
        // Parameter: bool parseData 是否要解析设备传递过来的数据(保存数据的时候,不需要解析)
        bool LoadAllData(bool useFastSync, bool parseData);

        // Summary  : 同步前的准备
        // Returns  : bool 成功返回TRUE
        // Parameter: bool useFastSync 是否使用快速同步模式
        bool PrepareSync(bool useFastSync);

        //map<string,string> BuildActions(vector<string> linkEntityNamesKey, bool linkAllRecordsOfPulledEntityTypeSentKey);

        // 保存变更后数据到设备,继承类需要实现自己需要的保存方法
        virtual void DoSaveChanges() = 0;

        // 继承类需要清除解析类缓存数据,如果有
        virtual void ClearCachedData() = 0;

        // 解析设备传送过来的数据,继承类需要实现自己的解析方法
        virtual void ParsePlistEntities() = 0;

        // 释放发送到设备的数据
        void ReleaseSendInfo();

    private:
        // 释放从设备接受来的数据
        void ReleaseEntities();

    protected:
        string  _shortDataClass;    // 继承类需要提供DataClass对应的短名,用于判断是否可以同步用,DataClass最后的词
        string  _dataClass;         // 继承类需要提供当前要同步的DataClass全名,格式类似:com.apple.xxx
        string  _dataClassAnchor;   // 继承类需要提供当前DataClass用到的同步Anchor,用于和设备匹配同步时间用
        int     _dataClassVersion;  // 继承类需要提供当前支持的DataClass版本,一般是10x 这样的(不同固件,不同dataclass,这个值不一样,不过助手发统一的值给设备,当前没有问题.)
//         bool    _useCache;          // 是否使用缓存,当前只有联系人实现
//         bool    _isCacheExpired;    // 是否重新加载数据
        map< string, map<string,void*> > _mapEntities;
        vector<void*> _sendInfo;
        map<string, string> _actions; // 一般传送一个指示设备是否要合并相应条目参数

        DeviceSyncClient* _syncClient;
        IosUsbConnection* _device;
        const DeviceData* _pDevice;
    };
}
