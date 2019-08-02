#pragma once

#include "Monitor/ProgressSubject.h"
#include "NodeData.h"
#include "RootData.h"

namespace ND91Assistant
{
    // �ļ������ࣺ����ִ�и����ļ�����
    // ��������Ķ��Ǿ�̬�������༴���ʺϹ鵽�����ڵ���ķ���
	class CLASSINDLL_CLASS_DECL FileHelper
	{
	public:

        // Summary  : �õ�ָ��·����Ӧ�Ľڵ�
        // Returns  : NodeData*         : �ڵ�ָ�룬����NULL��ʾ�Ҳ���
        // Parameter: DirData * pRoot   : ��Ŀ¼
        // Parameter: wstring path      : ·��
        // Parameter: bool bExactMatch  : �Ƿ���ȫƥ��
        static NodeData* GetNode(DirData* pRoot, wstring path, bool bFindLinkTarget = true, bool bExactMatch = true);


		// Summary  : �õ�ָ��·����Ӧ�Ľڵ�,�м������ӵĻ�ֱ�ӽ������ӵ�ַ
		// Returns  : NodeData*         : �ڵ�ָ�룬����NULL��ʾ�Ҳ���
		// Parameter: DirData * pRoot   : ��Ŀ¼
		// Parameter: wstring path      : ·��
		static NodeData* GetRealNode(DirData* pRoot, wstring path);

        // Summary:   ���ص�ָ��Ŀ¼��Ҫ�����нڵ㶼����ͬһ���豸��
        // Returns:   ������
        // Parameter: vector<NodeData*>* pNodes     : Ҫ���ص����нڵ�
        // Parameter: wstring destDir               : ���ص���Ŀ¼
        // Parameter: ProgressObserver* pObserver   : ���ؽ��Ƚ�����
        static TransferResult Download(vector<NodeData*>* pNodes, wstring destDir,ProgressObserver* pObserver);

        // Summary:   ����ɾ���ӽڵ�
        // Returns:   NULL: �ɹ� ��NULL: ɾ��ʧ�ܵĽڵ�
        // Parameter: vector<NodeData * > * pNodes  : Ҫɾ�������нڵ�
        // Parameter: ProgressObserver * pObserver  : ���Ƚ�����
        static NodeData* BatchDelete(vector<NodeData*>* pNodes, ProgressObserver* pObserver);

        // Summary:   ��·��ɾ��һ���ļ���Ŀ¼���޽���֧��
        // Returns:   �Ƿ�ɹ�
        // Parameter: const DeviceData * pDevice    : �豸ָ��
        // Parameter: wstring path                  : ����·��
        static bool Delete(const DeviceData* pDevice, wstring path);

        // Summary:   ͳ��ָ���ڵ���ļ�Ŀ¼�����ʹ�С�ܺͣ������õ�pSubject��
        // Returns:   void
        // Parameter: vector<NodeData*>*  pNodes    : �ڵ�����
        // Parameter: DownloadFileCommand & cmd     : �洢����������
        static void SetTaskSize(vector<NodeData*>* pNodes, ProgressSubject* pSubject,ProgressObserver* pObserver);

        // Summary  : pc�ļ���Ŀ¼, �ϴ���ָ���豸·��
        // Returns  : �Ƿ�ɹ�
        // Parameter: const DeviceData * pDevice    : �豸ָ��
        // Parameter: wstring sourcePath            : ����pc·��, �ļ���Ŀ¼
        // Parameter: wstring destDir               : �豸�ϵ�Ŀ¼
        // Parameter: ProgressObserver * pObserver  : ���Ƚ�����
        static bool Upload(const DeviceData* pDevice, wstring sourcePath, wstring destDir, ProgressObserver* pObserver, bool bSetAttr = false);

        // ��Ϊ��Ҫ����ʧ�ܻ���ȡ��������һ��������������ֵ��ͬ
        static TransferResult Upload2(const DeviceData* pDevice, wstring sourcePath, wstring destDir, ProgressObserver* pObserver, bool bSetAttr = false);

        // Summary  : ���豸�ϴ���ָ��·��
        // Returns  : �����ɹ����Ѵ��ڷ��ؽڵ�ָ��, ʧ�ܷ���NULL
        // Parameter: const DeviceData * pDevice    : �豸ָ��
        // Parameter: wstring dirInDevice           : �豸�ϵ�Ŀ¼
        static DirData* CreateDir(const DeviceData* pDevice, wstring dirInDevice);

        // Summary  : ��ȡӦ��Ŀ¼�ڵ��ڽ�����ʾ������
        // Returns  : void
        // Parameter: NodeData * pNode Ӧ��Ŀ¼�ڵ�
        // Parameter: const DeviceData * pDevice
        static void IosGetDisplayName(NodeData* pNode, const DeviceData* pDevice);
	};
}
