/************************************************************************/
/* ���			                                                        */
/* Copyright : Kingsoft 2005										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2004.12.30	Create												*/
/************************************************************************/
#ifndef _KPLAYER_H_
#define	_KPLAYER_H_

#include <list>
#include "SO3ProtocolBasic.h"
#include "KCharacter.h"
#include "KCircleList.h"
#include "KSkillList.h"
#include "KProfessionList.h"
#include "KRecipeList.h"
#include "KCDTimerList.h"
#include "KQuestInfoList.h"
#include "KQuestList.h"
#include "KGPQ.h"
#include "KItemList.h"
#include "KRoleDBDataDef.h"
#include "KPatrolPath.h"
#include "KReputation.h"
#include "KPKController.h"
#include "KTradingBox.h"
#include "KUserPreferences.h"
#include "KBookList.h"
#include "KSkillRecipeList.h"
#include "KCustomData.h"

#ifdef _SERVER
#include "KScriptTimerList.h"
#endif //_SERVER

#define MAX_POSITION_RECORD	16
#define PLAYER_SCRIPT SCRIPT_DIR"/player/PlayerScript.lua"

class IKG_Buffer;
class S2C_PLAYER_DISPLAY_DATA;

enum GAME_STATUS
{
	gsInvalid = 0,			    // ��Ч״̬

	gsWaitForConnect,			// ���ڵȴ��ͻ��˵�����
	gsWaitForPermit,		    // ��½ʱ���ڵȴ�GC��ȷ��
    gsWaitForRoleData,		    // ��¼ʱ���ڵȴ�Goddess�Ļذ�
    gsWaitForLoginLoading,      // ��¼(����)�����еȴ��ͻ�������
	gsWaitForNewMapLoading,     // �л���ͼ�����еȴ��ͻ�������

	gsPlaying,				    // ������Ϸ��

	gsSearchMap,			    // ����������ͼ,��һ����Ҫ���
    gsSaveDataForTransmission,  // ��������׼�����
	gsTransmission,			    // ���ʱ���ڴ����ɫ����

	gsDeleting,				    // �ͻ��˶����������ʱ�ǳ�
	gsWaitForDisconnect,	    // ���ʱԴ�������ȴ��ͻ�����������

	gsTotal
};

struct KWindowSelectList
{
	KTarget			Target;
	DWORD			dwScriptID;		//�ű�ID
	DWORD			dwIndex;		//��ˮ��
	int				nFuncCount;		//��������
	DWORD			dwMapID;
	int			    nMapCopyIndex;
	int				nX;
	int				nY;
	int				nZ;
	char			szFuncName[MAX_WINDOW_SELECT_COUNT][MAX_SELECT_FUNCNAME_LEN];
};

enum KCHARACTER_REVIVE_STATE
{
    crsInvalid = 0,
    crsWaitingForRevive,     // ����״̬��,�ȴ�����ʱ�䵽�����Ǳ�����
    crsConfirmRevive,        // ��������,AҪ����B,��B��δ�ظ�
    crsTotal
};

struct KPLAYER_REVIVE_PARAM
{
    int                     nSituReviveFrame;          // ����ԭ�ظ����֡
    time_t                  nLastSituReviveTime;       // �ϴ�ԭ�ظ���ʱ��
    int                     nSituReviveCount;          // ԭ�ظ��������, ���ڼ�����������ͷ�ʱ��
    DWORD                   dwReviver;                 // �����A�����ʱ��,�����ʾA��ID,����XYZ��A������
    int                     nX;                        
    int                     nY;
    int                     nZ;
    int                     nReviveLife;
    int                     nReviveMana;
    KCHARACTER_REVIVE_STATE eState;
};

#define KPLAYER_REVIVE_CTRL_UI_TEXT_LEN 32

struct KPLAYER_REVIVE_CTRL 
{
    int     nNextCheckReviveFrame;  // �������´μ�鸴��������ʱ��
    int     nReviveFrame;           // �����ʱ���ᱻͬ��������

    BOOL    bReviveInSitu;
    BOOL    bReviveInAltar;
    BOOL    bReviveByPlayer;

    time_t  nLastSituReviveTime;    // �ϴ�ԭ�ظ���ʱ��
    int     nSituReviveCount;       // ԭ�ظ��������, ���ڼ�����������ͷ�ʱ��
};

// ��ҽӵ�����֪ͨ���Ӧ����
enum KDEATH_RESPOND_CODE
{
    drcInvalid,
    drcReviveInSitu,        // ԭ�ظ���
    drcRevivedByPlayer,     // ����Ҹ���
    drcRevivedInAltar,      // ����㸴��      
    drcTotal
};

// ����ͬ����Ʒ��Ϣ
struct TALK_SYNC_ITEM
{
    unsigned uDataSize;
    BYTE byData[0];
};

struct TALK_SYNC_ITEM_INFO
{
    int     nVersion;
    DWORD   dwTabType;
    int     dwIndex;
};

struct TALK_SYNC_QUEST 
{
    DWORD   dwQuestID;
};

struct TALK_SYNC_RECIPE 
{
    DWORD   dwCraftID;
    DWORD   dwRecipeID;
};

struct TALK_SYNC_ENCHANT 
{
    DWORD   dwProID;
    DWORD   dwCraftID;
    DWORD   dwRecipeID;
};

struct TALK_SYNC_SKILL_RECIPE 
{
    DWORD   dwID;
    DWORD   dwLevel;
};

struct TALK_SYNC_BOOK
{
    TALK_SYNC_ITEM_INFO ItemInfo;
    int   nBookRecipeID;
};

struct KROLE_POSITION
{
    DWORD	dwMapID;
    int	    nMapCopyIndex;
    int		nX;
    int		nY;
    int		nZ;
    int     nFaceDirection;
};

struct KSHARE_QUEST_RESULT
{
    DWORD dwQuestID;
    DWORD dwTargetPlayerID;
};

struct KMOVE_CTRL 
{
    BOOL bDisable;      // ��ֹ�������������
    BOOL bMove;         // �Ƿ����ƶ�
    BOOL bWalk;         // �Ƿ���Walk(������Run,����Swim)
    int  nDirection8;   // �ƶ��ķ���,���Face direction,ֻ��8��������ֵ[0,7]
    int  nTurn;         // 0, ��ת,1,˳ת,-1��ת
    int  nMoveCount;    // �ͻ����ƶ�����������
    BOOL bUpdate;
};

class KPlayer : public KCharacter
{
public:
	KPlayer();
	~KPlayer();

	BOOL Init(void);
	void UnInit(void);

	BOOL Activate(void);

private:
#ifdef _SERVER
    void ScriptActivate();
#endif

public:
	BOOL OpenDoodad(KDoodad* pDoodad);

    void MoveCtrl();

public:
	GUID				m_Guid;

#if defined(_SERVER)
	int					m_nConnIndex;
	char				m_szAccount[_NAME_LEN];
	tagExtPointInfo		m_ExtPointInfo;
	time_t	            m_nlLeftTime;
    int                 m_nLastClientFrame;
    int                 m_nNextSaveFrame;
	DWORD				m_dwRoundRollCount;  // �������ʰȡ�ļ�����

	time_t				m_nLastSaveTime; 
    time_t              m_nLastLoginTime;
    time_t              m_nCurrentLoginTime;
    time_t              m_nTotalGameTime;
    time_t              m_nCreateTime;

	KCustomData<128>    m_CustomData;

	KScriptTimerList<KPlayer, 16>		m_ScriptTimerList;
    
    // ��ʾ��ҽ������͵���һ����ڵ��ʱ�䣬Ϊ0��ʾ���贫��
    time_t              m_nBanishTime;
    KROLE_POSITION      m_LastEntry;
#endif

#if defined(_CLIENT)
    char                m_szTongName[_NAME_LEN];
#endif

    KMOVE_CTRL          m_MoveCtrl;

	ROLE_TYPE			m_eRoleType;

	GAME_STATUS			m_eGameStatus;
	int					m_nTimer;

	KQuestList			m_QuestList;
    KGPQList            m_PQList;
	KSkillList			m_SkillList;

	KProfessionList		m_ProfessionList;
	KRecipeList			m_RecipeList;

	// ��ҵ�Cooldown��ʱ����
	KCDTimerList		m_TimerList;
	// �������
	KItemList			m_ItemList;			//��ҵĵ��߼��ϣ�װ���ġ��������ġ��������

	// �������
	KReputation			m_ReputeList;		//��������

	KPKController		m_PK;				//PK��صĿ���

	KWindowSelectList	m_SelectList;		//ѡ��ص������б�

	KUserPreferences	m_UserPreferences;	//  ��ҽ������ݴ���

	KBookList			m_BookList;			//  ��ҵĶ������
    // ������������ID
    DWORD               m_dwSystemTeamID;
    DWORD               m_dwTeamID;

    DWORD GetCurrentTeamID();
    int GetCurrentTeamGroupIndex();

    int                 m_nBattleFieldSide; // ս������

#ifdef _SERVER
    int                 m_nExpPercent;          // ɱ�ֺ��þ���ֵ��ϵ��
    int                 m_nReputationPercent;   // ɱ�ֺ�������ֵ��ϵ��
#endif
	
	int	                m_nExperience;			// ��ɫ����
	
	DWORD				m_dwBankNpcID;          // �����е�Npc

    KTradingBox*        m_pTradingBox;           
    DWORD               m_dwTradingInviteDst;   // ���������Ŀ�����
    DWORD               m_dwTradingInviteSrc;   // �����׵����
    
    BOOL                m_bOnHorse;             // �Ƿ�������״̬
	// ����װ������
    WORD				m_wRepresentId[perRepresentCount];
    DWORD               m_dwRepresentIdLock;    // ��perRepresentCountλ���������������

    DWORD               m_dwSchoolID;           // ���ڴ洢װ���ڹ������ɣ�����һϵ�еı����ж�
    
    // ----------------- ������� ---------------------------->
    BOOL				m_bOnPracticeRoom;          // �Ƿ���������

    KSkillRecipeList    m_SkillRecipeList;

    // ----------------- ��Ϊ��� -------------------------------->
    int                 m_nCurrentTrainValue;       // ��Ϊ
    int                 m_nMaxTrainValue;           // �����Ϊֵ
    int                 m_nUsedTrainValue;          // �Ѿ����뾭������Ϊֵ
#ifdef _SERVER
    int                 m_nAddTrainTimeInToday;     // �����Ѿ��ӵ���Ϊ��ʱ��
#endif

    // ----------------- �ɾ���� -------------------------------->
    KCustomData<MAX_ACHIEVEMENT_ID / CHAR_BIT>  m_AchievementSet;
    int                                         m_nAchievementPoint;    // �ɾͻ���
    int                                         m_nAchievementRecord;   // ��Ϸ��ɶ�

    // ----------------- ��ͨ�����ĳɹ��� ------------------------>
    int                 m_nBaseVenationCof;         // ����ϵ
    int                 m_nPhysicsVenationCof;      // �⹦ϵ
    int                 m_nMagicVenationCof;        // �ڹ�ϵ
    int                 m_nAssistVenationCof;       // ����ϵ

#ifdef _SERVER
    // ----------------- BUFF˥����� ------------------------>
    int                 m_nBuffDecayCount[MAX_BUFF_DECAY_TYPE];
    int                 m_nBuffDecayNextFrame[MAX_BUFF_DECAY_TYPE];
#endif //_SERVER

#ifdef _CLIENT
	// ----------------- С��ͼ�״� -------------------------->
	MINIMAP_RADAR_TYPE	m_eRadarType;				//�״�����
	int					m_nRadarParam;				//�״����
#endif

    typedef std::vector<int /* nRouteNode */> KOPEN_ROUTE_NODE_VECTOER;

    KOPEN_ROUTE_NODE_VECTOER m_OpenRouteNodeList;
    
    // ��ҷ��ʳ����ļ�¼
    KCustomData<MAX_MAP_ID_DATA_SIZE> m_SceneVisitRecord;
    
    //  ------------ ɱ��ֵ��� ----------------------------------->
    int     m_nCurrentKillPoint;        // ɱ��ֵ, ���ֵ���ܳ���65536
#ifdef _SERVER
    time_t  m_nLastKillPointReduceTime; // ��һ��ɱ��ֵ˥��ʱ��

    BOOL    m_bAbradeEquipmentDouble;   // �����к����ϵ�װ����˫���;�
    BOOL    m_bAbradeEquipmentAll;      // �����к����ϵ�װ���۹��;�
    BOOL    m_bAttackByGuard;           // ������NPC����
#endif
    BOOL    m_bOnlyReviveInSitu;        // ������ԭ�ظ���ĳ�����ֻ��ԭ�ظ���    
    BOOL    m_bCannotDialogWithNPC;     // ������NPC�Ի�
    BOOL    m_bRedName;                 // ����
    //  ------------------------------------------------------------>
    DWORD   m_dwTongID;
    int     m_nContribution;

    int     m_nMaxLevel;
    BOOL    m_bHideHat;

    // -------------- �������Լӳ� --------------------------------->
    int     m_nVitalityToParryValueCof;
    int     m_nVitalityToMaxLifeCof;
    int     m_nVitalityToPhysicsAttackPowerCof;
    int     m_nVitalityToSolarAttackPowerCof;
    int     m_nVitalityToLunarAttackPowerCof;
    int     m_nVitalityToNeutralAttackPowerCof;
    int     m_nVitalityToPoisonAttackPowerCof;
    int     m_nVitalityToTherapyPowerCof;
    int     m_nVitalityToPhysicsShieldCof;
    int     m_nSpiritToMaxManaCof;
    int     m_nSpiritToSolarCriticalStrikeCof;
    int     m_nSpiritToLunarCriticalStrikeCof;
    int     m_nSpiritToNeutralCriticalStrikeCof;
    int     m_nSpiritToPoisonCriticalStrikeCof;
    int     m_nSpiritToPhysicsAttackPowerCof;
    int     m_nSpiritToPhysicsCriticalStrikeCof;
    int     m_nSpiritToManaReplenishCof;
    int     m_nSpiritToSolarMagicShieldCof;
    int     m_nSpiritToLunarMagicShieldCof;
    int     m_nSpiritToNeutralMagicShieldCof;
    int     m_nSpiritToPoisonMagicShieldCof;
    int     m_nSpunkToManaReplenishCof;
    int     m_nSpunkToSolarAttackPowerCof;
    int     m_nSpunkToLunarAttackPowerCof;
    int     m_nSpunkToNeutralAttackPowerCof;
    int     m_nSpunkToPoisonAttackPowerCof;
    int     m_nStrengthToPhysicsCriticalStrikeCof;
    int     m_nStrengthToPhysicsAttackPowerCof;
    int     m_nAgilityToPhysicsCriticalStrikeCof;

#ifdef _SERVER
public:
    // ��Ұͬ�����
    void SyncObjectView();
    void RegisterViewObject(KRegion* pFromRegion, KRegion* pToRegion);

    struct KViewObject 
    {
        KViewObject() {};
        KViewObject(DWORD dwID, int nFrame) : dwObjectID(dwID), nViewFrame(nFrame) {};
        DWORD dwObjectID;
        int   nViewFrame;
    };

    typedef std::set<DWORD, std::less<DWORD>, KMemory::KAllocator<DWORD> > KVIEW_SYNC_TABLE;
    typedef std::list<KViewObject, KMemory::KAllocator<KViewObject> > KVIEW_SYNC_QUEUE;

    KVIEW_SYNC_TABLE    m_PlayerSyncTable;
    KVIEW_SYNC_QUEUE    m_PlayerSyncQueue;

    KVIEW_SYNC_TABLE    m_NpcSyncTable;
    KVIEW_SYNC_QUEUE    m_NpcSyncQueue;

    KVIEW_SYNC_TABLE    m_DoodadSyncTable;
    KVIEW_SYNC_QUEUE    m_DoodadSyncQueue;
#endif

public:
#ifdef _CLIENT
	void SetDisplayData(S2C_PLAYER_DISPLAY_DATA& rPlayerDisplayData);
#endif

    void SetGenderByRoleType(int nRoleType);

#ifdef _SERVER
    BOOL    Load(BYTE* pbyData, size_t uDataLen);
    BOOL    LoadExtRoleData(BYTE* pbyData, size_t uDataLen);
    BOOL    LoadSkillRecipeList(BYTE* pbyData, size_t uDataLen);
    BOOL    LoadQuestData(BYTE* pbyData, size_t uDataLen);
    BOOL    LoadStateInfo(BYTE* pbyData, size_t uDataLen);
    BOOL    LoadRoadOpenNode(BYTE* pbyData, size_t uDataLen);
    BOOL    LoadHeroData(BYTE* pbyData, size_t uDataLen);
    BOOL    LoadAchievementData(BYTE* pbyData, size_t uDataLen);

    BOOL    CallLoginScript();
    BOOL    RefreshDailyVariable(int nDays);

    BOOL    Save(size_t* puUsedSize, BYTE* pbyBuffer, size_t uBufferSize);
    BOOL    SaveSkillRecipeList(size_t* puUsedSize, BYTE* pbyBuffer, size_t uBufferSize);
    BOOL    SaveQuestStateAndList(size_t* puUsedSize, BYTE* pbyBuffer, size_t uBufferSize);
    BOOL    SaveStateInfo(size_t* puUsedSize, BYTE* pbyBuffer, size_t uBufferSize);
    BOOL    SaveRoadOpenNode(size_t* puUsedSize, BYTE* pbyBuffer, size_t uBufferSize);
    BOOL    SaveHeroData(size_t* puUsedSize, BYTE* pbyBuffer, size_t uBufferSize);
    BOOL    SaveAchievementData(size_t* puUsedSize, BYTE* pbyBuffer, size_t uBufferSize);
    BOOL    SavePosition();
    // �����������֮ǰӦ��ȷ��m_SavePosition����ȷ������(����ͨ��SavePosition)
    BOOL    SaveBaseInfo(KRoleBaseInfo* pBaseInfo);
    BOOL    LoadBaseInfo(KRoleBaseInfo* pBaseInfo);

    // �����ɫ����,������Loadʱ,��Ҫ����ҵ�λ����Ϣ���浽����ṹ����,��Ϊ������
    // ��Ҫ���ʵ�Scene,Cell��ָ�����Ϊ��
	KROLE_POSITION m_SavePosition;

    BOOL SwitchMap(DWORD dwMapID, int nCopyIndex, int nX, int nY, int nZ);
#endif

public:
#ifdef _SERVER
    BOOL CheckRevive(BOOL bFirstCheck);
    BOOL Revive(int nMode);
#endif

#ifdef _SERVER
	BOOL	CallDeathScript(DWORD dwKiller);
	void	ReplenishAttribute();		// �ָ�����,��Ҫ�����ר�еľ������
#endif	//_SERVER

	void AddExp(int nExpIncrement);
#ifdef _SERVER
    BOOL AddContribution(int nContribution);
#endif
	void SetLevel(int nLevel);	

	virtual BOOL ChangeRegion(KRegion* pDestRegion);
#ifdef _SERVER
    void SetCell(KCell* pDstCell);

    BOOL CallTrapScript(DWORD dwScript, const char* szFunction);

	DWORD m_dwLastScriptID;
#endif

	int GetPlayerRelation(KPlayer* pTarget);
	int GetNpcRelation(KNpc* pTarget);
    
#ifdef _CLIENT
    // �ú�������: ���ͻ�����ҵ�ǰû��Ŀ��, ����һ��Npc��Player��ͼ�����ͷ�һ���к�����ʱ,
    // �ͻ�����Ҿͻ�ѵ�ǰĿ���л�����ͼ��������Npc��Player
    BOOL SwitchSelectTarget(DWORD dwSkillSrcID);
#endif

	//------------------------ ����� ------------------------------->
	int	m_nCurrentStamina;
    int m_nCurrentThew;
    
    int	m_nMaxStamina;
    int m_nMaxThew;

	BOOL CostStamina(int nStamina);
    BOOL CostThew(int nThew);

	BOOL LearnProfession(DWORD dwProfessionID);
	BOOL LearnBranch(DWORD dwProfessionID, DWORD dwBranchID);
	BOOL ForgetProfession(DWORD dwProfessionID);
	BOOL AddProfessionProficiency(DWORD dwProfessionID, DWORD dwExp);
	BOOL SetMaxProfessionLevel(DWORD dwProfessionID, DWORD dwLevel);

	BOOL CanLearnRecipe(DWORD dwCraftID, DWORD dwRecipeID, DWORD dwMasterID);
	BOOL LearnRecipe(DWORD dwCraftID, DWORD dwRecipeID);
	BOOL ForgetRecipe(DWORD dwProfessionID);

	CRAFT_RESULT_CODE CanCastProfessionSkill(DWORD dwCraftID, DWORD dwRecipeID, DWORD dwRBookItemID, KTarget& Target);
	CRAFT_RESULT_CODE CastProfessionSkill(DWORD dwCraftID, DWORD dwRecipeID, DWORD dwRBookItemID, KTarget& Target);
	//<----------------------- ����� --------------------------------

#ifdef _CLIENT
    BOOL CanFinishQuestOnNpc(KNpc* pNpc);
    BOOL CanFinishTheQuestOnNpc(DWORD dwQuestID, KNpc* pNpc);
    QUEST_DIFFICULTY_LEVEL GetQuestDiffcultyLevel(DWORD dwQuestID); // ȡ����������׶�
#endif

#ifdef _CLIENT
    BOOL CopyTalkData(DWORD dwTalkerID, size_t uSize, BYTE* pbyData);
#endif

private:

#ifdef _CLIENT
    static BYTE     s_byTalkData[MAX_CHAT_TEXT_LEN];
    static unsigned s_uTalkDataSize;
    static BOOL     s_bFilterTalkText;
#endif

public:
    int GetCDValue(DWORD dwCooldownID);

    // �书װ����������: Client->LuaMountKungfu->server->MountKungfu->Client->MountKungfu .
    BOOL    MountKungfu(DWORD dwID, DWORD dwLevel);
    BOOL    UmountKungfu();
#if defined _CLIENT
	int IsEquiped(KCustomEquipInfo *pInfo);
#endif

    // ----------- λ���ƶ�����ʷ��¼ ------------------
#if defined(_SERVER)

    // ע��: ʲô�������ܷ������"��ʷ��¼"����?
    // ������ʷ��¼����Ĳ�������ζ�Żᷢ�����,��ô��Ȼ:
    // ----����ĳ������X,�����1000֡�ع���990֡��,����1000֡ʱ����ֵ�Ժ��Ӵ˶�ʧ���߷��������,�Ͳ��ܻع�
    // ----����: ���ǻᱻ����Buff�޸�(��������޸�)�Ķ���,���ǲ��ܷ���������ع���,��������m_nMaxLife, m_nRunSpeed�ȵ�...

    KPOSITION_RECORD    m_PositionRecord[MAX_POSITION_RECORD];
	int                 m_nVirtualFrame;   
	int                 m_nRecordCount;

    void RecordPosition();
    BOOL ReversePosition(int nFrame);
    BOOL CheckMoveAdjust(int nClientFrame, const KMOVE_CRITICAL_PARAM& crParam);
#endif

	BOOL ForwardPosition(int nFrame);

#ifdef _SERVER
    // ������ֵ�ϴ�ͬ��ֵ
	int		m_nLastSyncSelfMaxLife;
	int		m_nLastSyncSelfMaxMana; 
    int     m_nLastSyncSelfMaxStamina;
	int		m_nLastSyncSelfCurrentLife;
	int		m_nLastSyncSelfCurrentMana; 
    int     m_nLastSyncSelfCurrentStamina;
    int     m_nLastSyncSelfCurrentThew;
    int     m_nLastSyncSelfUpperWeak;
    int     m_nLastSyncSelfMiddleWeak;
    int     m_nLastSyncSelfLowerWeak;
	// Ŀ����ֵ����һ��ͬ��ֵ
	int		m_nLastSyncTargetMaxLife;
	int		m_nLastSyncTargetMaxMana; 
	int		m_nLastSyncTargetLifePercent;
	int		m_nLastSyncTargetManaPercent; 
    int     m_nLastSyncTargetUpperWeak;
    int     m_nLastSyncTargetMiddleWeak;
    int     m_nLastSyncTargetLowerWeak;
    DWORD   m_dwLastSyncTargetTargetID;
    int		m_nLastSyncTargetTargetMaxLife;
    int		m_nLastSyncTargetTargetMaxMana; 
    int		m_nLastSyncTargetTargetLifePercent;
    int		m_nLastSyncTargetTargetManaPercent; 
    // ͬ����������һ��ͬ����ֵ
    int     m_nLastSyncLocalPartyLifePercent;
    int     m_nLastSyncLocalPartyManaPercent;
    DWORD   m_dwLastSyncLocalPartyMemberMapID;
    int     m_nLastSyncLocalPartyMemberMapCopyIndex;
    int     m_nLastSyncLocalPartyMemberPosX;
    int     m_nLastSyncLocalPartyMemberPosY;
    // ���ͬ�����ݵ���һ��ͬ����ֵ
    int     m_nLastSyncGlobalPartyLifePercent;
    int     m_nLastSyncGlobalPartyManaPercent;
    DWORD   m_dwLastSyncGlobalPartyMemberMapID;
    int     m_nLastSyncGlobalPartyMemberMapCopyIndex;
    int     m_nLastSyncGlobalPartyMemberPosX;
    int     m_nLastSyncGlobalPartyMemberPosY;
    // Ŀ���ϴ�Buffͬ��
    DWORD	m_dwTargetBuffCRC;
    DWORD	m_dwTargetTargetBuffCRC;
    // Ŀ���ϴε���������
    DWORD   m_dwTargetDropID;

    void DoCycleSynchronous();
    void ResetPartyLastSyncParamRecord();
#endif

    BOOL RideHorse();
    BOOL DownHorse();
    
#ifdef _SERVER
    BOOL AddTrain(int nTrain);
    BOOL AddTrainNoLimit(int nTrain);
    BOOL ShareQuest(DWORD dwQuestID);
#endif
    
#ifdef _CLIENT
    DWORD m_dwFormationEffectID;
    int   m_nFormationEffectLevel;
#endif

    BOOL IsFormationLeader(); // ����
#ifdef _SERVER
    BOOL CanSetFormationLeader();

    BOOL SyncFormationCoefficient();
    BOOL ProcessTeamMemberAttraction();
    BOOL ProcessTeamMemberOnlyGroupAttraction();
#endif
    KPLAYER_REVIVE_PARAM    m_ReviveParam;
    KPLAYER_REVIVE_CTRL     m_ReviveCtrl;

#ifdef _SERVER
    time_t  m_nBanTime;
#endif

    BOOL CanAddFoe();
    BOOL IsFoe(DWORD dwDestPlayerID); // �Ƿ����ҵĳ����б���

// ��Ӫ----------------------------------->
public:
    KCAMP m_eCamp;
    int   m_nCurrentPrestige; //  ����ֵ

    BOOL SetCamp(KCAMP eNewCamp);
#ifdef _SERVER
    int  m_nClientCampInfoVersion;

    BOOL AddPrestige(int nAddPrestige);
    void ProcessCampPK(DWORD dwKillerID);
    void CallChangeCampScript();

private:
    int GetDiffLevelPercent(int nKillerLevel);
#endif
// <----------------------------------��Ӫ

// Ӣ�۸���----------------------------------->
public:
    BOOL m_bHeroFlag;

    int getHeroFlag(){return m_bHeroFlag;}
    void setHeroFlag(int nValue);

#ifdef _SERVER
    KCustomData<MAX_MAP_ID_DATA_SIZE> m_HeroMapCopyOpenFlag;
#endif
// <-----------------------------------Ӣ�۸���

    // �ɾ����
#ifdef _SERVER
    BOOL AcquireAchievement(int nID);
    BOOL AddAchievementPoint(int nDeltaPoint);
#endif
    
public:
	DECLARE_LUA_CLASS(KPlayer);

#ifdef _SERVER
    DECLARE_LUA_STRING(Account, sizeof(m_szAccount));
    DECLARE_LUA_DWORD(KillerID);
    DECLARE_LUA_DWORD(LastScriptID);
#endif

    DECLARE_LUA_ENUM(Camp);
    DECLARE_LUA_INTEGER(CurrentPrestige);
	DECLARE_LUA_DWORD(TeamID);
	DECLARE_LUA_ENUM(RoleType);
	DECLARE_LUA_INTEGER(Experience);
    DECLARE_LUA_BOOL(OnPracticeRoom);
	DECLARE_LUA_INTEGER(CurrentStamina);
    DECLARE_LUA_INTEGER(CurrentThew);
    DECLARE_LUA_INTEGER(MaxStamina);
    DECLARE_LUA_INTEGER(MaxThew);
	DECLARE_LUA_DWORD(BankNpcID);
    DECLARE_LUA_BOOL(OnHorse);
    
    DECLARE_LUA_INTEGER(BattleFieldSide);

    DECLARE_LUA_DWORD(SchoolID);

    DECLARE_LUA_INTEGER(CurrentTrainValue);
    DECLARE_LUA_INTEGER(MaxTrainValue);
    DECLARE_LUA_INTEGER(UsedTrainValue);
#ifdef _SERVER
    DECLARE_LUA_INTEGER(AddTrainTimeInToday);
#endif

    DECLARE_LUA_INTEGER(BaseVenationCof);
    DECLARE_LUA_INTEGER(PhysicsVenationCof);
    DECLARE_LUA_INTEGER(MagicVenationCof);
    DECLARE_LUA_INTEGER(AssistVenationCof);

#ifdef _SERVER
    DECLARE_LUA_STRUCT_INTEGER(Reviver, m_ReviveParam.dwReviver);
    DECLARE_LUA_STRUCT_INTEGER(ReviveX, m_ReviveParam.nX);
    DECLARE_LUA_STRUCT_INTEGER(ReviveY, m_ReviveParam.nY);
    DECLARE_LUA_STRUCT_INTEGER(ReviveZ, m_ReviveParam.nZ);
    DECLARE_LUA_STRUCT_INTEGER(ReviveLife, m_ReviveParam.nReviveLife);
    DECLARE_LUA_STRUCT_INTEGER(ReviveMana, m_ReviveParam.nReviveMana);
#endif

#ifdef _CLIENT
    DECLARE_LUA_DWORD(FormationEffectID);
    DECLARE_LUA_INTEGER(FormationEffectLevel);

	DECLARE_LUA_BOOL(HideHat);
#endif
    
    DECLARE_LUA_INTEGER(CurrentKillPoint);
#ifdef _SERVER
    DECLARE_LUA_BOOL(AbradeEquipmentDouble);
    DECLARE_LUA_BOOL(AbradeEquipmentAll);
    DECLARE_LUA_BOOL(AttackByGuard);
#endif
    DECLARE_LUA_BOOL(OnlyReviveInSitu);
    DECLARE_LUA_BOOL(CannotDialogWithNPC);
    DECLARE_LUA_BOOL(RedName);
    DECLARE_LUA_DWORD(TongID);
    DECLARE_LUA_INTEGER(MaxLevel);
    DECLARE_LUA_INTEGER(Contribution);
    
	int LuaGetItem(Lua_State* L);
	int LuaDelItem(Lua_State* L);

	int LuaGetItemByIndex(Lua_State* L);
	int LuaGetEquipItem(Lua_State* L);

#ifdef _SERVER
    int LuaCanAddItem(Lua_State* L);
    int LuaAddItem(Lua_State* L);
	int LuaCostItem(Lua_State* L);
	int LuaAddMoney(Lua_State* L);
    int LuaAddContribution(Lua_State* L);
	int LuaOpenWindow(Lua_State* L);
    int LuaGetLastSaveTime(Lua_State* L);
    int LuaGetLastLoginTime(Lua_State* L);
    int LuaGetCurrentLoginTime(Lua_State* L);
#endif
	
	int LuaGetMoney(Lua_State* L);
	int LuaExchangeItem(Lua_State* L);

	int LuaEnableBankPackage(Lua_State* L);
	int	LuaGetBankPackageCount(Lua_State* L);
	int LuaIsBankPackageEnabled(Lua_State* L);

	int LuaDestroyItem(Lua_State* L);

	int LuaGetRepresentID(Lua_State* L);

#ifdef _SERVER
    int LuaSetRepresentID(Lua_State* L);
#endif
	
	int LuaGetBoxType(Lua_State* L);
	int LuaGetBoxSize(Lua_State* L);
	int LuaGetContainType(Lua_State* L);
	int LuaCanExchange(Lua_State* L);

	int LuaOpenBank(Lua_State* L);
	int LuaCloseBank(Lua_State* L);

	int LuaGetEquipPos(Lua_State* L);	//��ȡװ������װ���ĵط�������װ������nil
	int LuaGetFreeRoom(Lua_State* L);
	int LuaGetFreeRoomSize(Lua_State* L);
    int LuaGetBoxFreeRoomSize(Lua_State* L);
	int LuaGetFreeRoomInPackage(Lua_State* L);
	int LuaGetStackRoomInPackage(Lua_State* L);
	int LuaGetStackRoomInBank(Lua_State* L);
	int LuaSatisfyRequire(Lua_State* L);	//�Ƿ�����װ������
    
    int LuaGetItemAmountInAllPackages(Lua_State* L);
	int LuaGetItemAmount(Lua_State* L);
	
	int LuaGetSchoolList(Lua_State* L);
	int LuaGetKungfuList(Lua_State* L);
	int LuaGetSkillList(Lua_State* L);
	int LuaGetAllSkillList(Lua_State* L);
#ifdef _CLIENT
    int LuaGetSkillRecipeKey(Lua_State* L);
	int LuaGetSkillRecipeKeyCount(Lua_State* L);
#endif
	
	// ��⵱ǰ����Ƿ���Խ���ָ������
	int LuaCanAcceptQuest(Lua_State* L);
	// ��⵱ǰ����Ƿ�������ָ������
	int LuaCanFinishQuest(Lua_State* L);
	// ��ȡ��ǰ���ָ�������״̬
	int LuaGetQuestState(Lua_State* L);
	// ��ȡ����׶� -1���쳣 0����û������ 1���ѽ�δ��� 2����ɵ�û�� 3���Ѿ�������
	int LuaGetQuestPhase(Lua_State* L);
	// ��ȡ�����б�
	int LuaGetQuestList(Lua_State* L);
	// ��ȡ�����б�
	int LuaGetQuestTree(Lua_State* L);
	// ��ȡ��������
	int LuaGetQuestIndex(Lua_State* L);
	// ȡ����ID
	int LuaGetQuestID(Lua_State* L);
	// ��ȡ������˥����Ϣ
	int LuaGetQuestExpAttenuation(Lua_State* L);

#ifdef _CLIENT
	int LuaGetQuestTraceInfo(Lua_State* L);
    int LuaGetQuestAssistDailyCount(Lua_State* L);
#endif

	int LuaSetQuestValue(Lua_State* L);
	int LuaGetQuestValue(Lua_State* L);

	int LuaSetQuestBitValue(Lua_State* L);
	int LuaGetQuestBitValue(Lua_State* L);

	// ����ɱNpc����
	int LuaSetKillNpcCount(Lua_State* L);
	// ��ȡɱNpc����
	int LuaGetKillNpcCount(Lua_State* L);
	// ��ȡ����ʣ��ʱ��
	int LuaGetQuestLastTime(Lua_State* L);
	// ��������ʧ�ܱ��
	int LuaSetQuestFailedFlag(Lua_State* L);
	// ��ȡ����ʧ�ܱ��
	int LuaGetQuestFailedFlag(Lua_State* L);

	int LuaAcceptQuest(Lua_State* L);
	int LuaFinishQuest(Lua_State* L);
	int LuaCancelQuest(Lua_State* L);
    int LuaClearQuest(Lua_State* L);
	int LuaShareQuest(Lua_State* L);
    int LuaGetFinishedDailyQuestCount(Lua_State* L);

#ifdef _SERVER
    int LuaForceFinishQuest(Lua_State* L);
#endif
#ifdef _CLIENT
	int LuaAcceptEscortQuest(Lua_State* L);
#endif
    int LuaRegisterQuestAssistance(Lua_State* L);
	
	int LuaGetProfession(Lua_State* L);
	int LuaGetProfessionLevel(Lua_State* L);
    int LuaGetProfessionMaxLevel(Lua_State* L);
    int LuaGetProfessionProficiency(Lua_State* L);
	int LuaGetProfessionBranch(Lua_State* L);
	int LuaForgetProfession(Lua_State* L);
	int LuaGetRecipe(Lua_State* L);
	int LuaCastProfessionSkill(Lua_State* L);
	int LuaGetBookList(Lua_State* L);
	int LuaGetBookSegmentList(Lua_State* L);
    int LuaIsBookMemorized(Lua_State* L);

	int LuaCanLearnSkill(Lua_State* L);
	int LuaLearnSkill(Lua_State* L);

#if defined(_CLIENT)
	int LuaLearnRecipe(Lua_State* L);
#endif

#if defined(_SERVER)
    int LuaSetLastEntry(Lua_State* L);
	int LuaSwitchMap(Lua_State* L);
    int LuaResetMap(Lua_State* L);
    int LuaAutoFly(Lua_State* L);
	int LuaAddExp(Lua_State* L);
    int LuaAddSkillExp(Lua_State* L);
    int LuaForgetSkill(Lua_State* L);

	int LuaLearnProfession(Lua_State* L);
	int LuaLearnBranch(Lua_State* L);
	int LuaAddProfessionProficiency(Lua_State* L);
	int LuaSetMaxProfessionLevel(Lua_State* L);

	int LuaLearnRecipe(Lua_State* L);

	int LuaAddStamina(Lua_State* L);
    int LuaAddThew(Lua_State* L);
	int LuaSetPartyLootMode(Lua_State* L);
	int LuaCanDoCustomOTAction(Lua_State* L);
    int LuaDoCustomOTAction(Lua_State* L);
#endif	//_SERVER

#if defined(_CLIENT)
    int LuaOpenBook(Lua_State* L);
#endif

	int LuaCanLearnProfession(Lua_State* L);
	int LuaCanLearnBranch(Lua_State* L);
	int LuaCanLearnRecipe(Lua_State* L);
    int LuaIsProfessionLearnedByCraftID(Lua_State* L);
	int	LuaIsRecipeLearned(Lua_State* L);
	
#if defined(_CLIENT)
	int LuaWindowSelect(Lua_State* L);
    int LuaDoDeathRespond(Lua_State* L);
#endif	//_CLIENT


#if defined(_CLIENT)
	int LuaOnCloseLootWindow(Lua_State* L);
#endif
    int LuaGetCommonSkill(Lua_State* L);
#if defined(_CLIENT)
    int LuaCastCommonSkill(Lua_State* L);
    int LuaStopCurrentAction(Lua_State* L);
#endif //_CLIENT

	int LuaGetItemCDProgress(Lua_State* L);

    int LuaGetSkillCDProgress(Lua_State* L);
    int LuaGetCDInterval(Lua_State* L);
    int LuaGetCDLeft(Lua_State* L);
    int LuaGetSkillLevel(Lua_State* L);

    int LuaGetSkillLifeCost(Lua_State* L);
    int LuaGetSkillManaCost(Lua_State* L);
    int LuaGetSkillStaminaCost(Lua_State* L);

    int LuaGetSkillPrepare(Lua_State* L);
    int LuaGetSkillChannel(Lua_State* L);

    int LuaGetSkillMinRadius(Lua_State* L);
    int LuaGetSkillMaxRadius(Lua_State* L);

    int LuaGetSkillExp(Lua_State* L);

    int LuaMountKungfu(Lua_State* L);
    int LuaUmountKungfu(Lua_State* L);
    int LuaGetKungfuMount(Lua_State* L);
	int	LuaGetOTActionState(Lua_State* L);

#ifdef _SERVER
    int LuaAddSkillRecipe(Lua_State* L);
    int LuaDelSkillRecipe(Lua_State* L);
#endif
    int LuaGetSkillRecipeList(Lua_State* L);
    int LuaIsSkillRecipeExist(Lua_State* L);
    int LuaCanLearnSkillRecipe(Lua_State* L);
    int LuaActiveSkillRecipe(Lua_State* L);
    int LuaDeactiveSKillRecipe(Lua_State* L);

	//--- AI ----------------------------------
#if defined(_CLIENT)
	int LuaFollowSelectTarget(Lua_State* L);

	void HostStopFollow();
	int LuaStopFollow(Lua_State* L);
#endif //_CLIENT

	int LuaSearchForAllies(Lua_State* L);
	int LuaSearchForEnemy(Lua_State* L);
	//-----------------------------------------

#ifdef _CLIENT
	int LuaUseItem(Lua_State* L);
	int LuaGetItemPos(Lua_State* L);
#endif

#ifdef _SERVER
    int LuaUseItemOnNoTarget(Lua_State* L);
    int LuaUseItemOnNpc(Lua_State* L);
    int LuaUseItemOnPlayer(Lua_State* L);
    int LuaUseItemOnItem(Lua_State* L);
    int LuaUseItemOnDoodad(Lua_State* L);
    int LuaUseItemOnCoordinates(Lua_State* L);
#endif

    int LuaSearchForPartyMember(Lua_State* L);

#if defined(_SERVER)
	int LuaAbradeEquipmentOnDeath(Lua_State* L);    

	int LuaSetReputation(Lua_State* L);
	int LuaAddReputation(Lua_State* L);
	int LuaOpenReputation(Lua_State* L);

	// ��ʱ��
	int LuaSetTimer(Lua_State* L);
	int LuaStopTimer(Lua_State* L);
	int LuaRestartTimer(Lua_State* L);
	int LuaRemoveTimer(Lua_State* L);
#endif

	int LuaGetReputation(Lua_State* L);
    int LuaGetReputeLevel(Lua_State* L);
	int LuaIsReputationHide(Lua_State* L);

#if defined(_CLIENT)
	int LuaSetFightFlag(Lua_State* L);
	int LuaGetFightFlag(Lua_State* L);
	int LuaIsFightFlagLocked(Lua_State* L);

    int LuaTradingInviteRequest(Lua_State* L);
    int LuaTradingInviteRespond(Lua_State* L);
    int LuaTradingAddItem(Lua_State* L);
    int LuaTradingDeleteItem(Lua_State* L);
    int LuaTradingSetMoney(Lua_State* L);
    int LuaTradingConfirm(Lua_State* L);

    int LuaGetTradingItem(Lua_State* L);
    int LuaGetTradingMoney(Lua_State* L);

    //--- Talk --------------------------------
    int LuaGetTalkData(Lua_State* L);
    int LuaGetTalkLinkItem(Lua_State* L);
    int LuaTalk(Lua_State* L);
    int LuaDoAnimation(Lua_State* L);

	int LuaIsInParty(Lua_State* L);
	int LuaIsPlayerInMyParty(Lua_State* L);
	int LuaIsPartyLeader(Lua_State* L);
	int LuaIsPartyFull(Lua_State* L);

	int LuaSetMinimapRadar(Lua_State* L);
	int LuaGetMiniMapRadar(Lua_State* L);
#endif

#ifdef _SERVER
    int LuaSendSystemMessage(Lua_State* L);
    int LuaGmSendMessage(Lua_State* L);
#endif
    //-----------------------------------------


    //--- ���� --------------------------------
    int LuaAddFellowship(Lua_State* L);
    int LuaDelFellowship(Lua_State* L);
    int LuaAddFellowshipGroup(Lua_State* L);
    int LuaDelFellowshipGroup(Lua_State* L);
    int LuaRenameFellowshipGroup(Lua_State* L);
    int LuaSetFellowshipRemark(Lua_State* L);
    int LuaCanAddFoe(Lua_State* L);
    int LuaAddFoe(Lua_State* L);
    int LuaDelFoe(Lua_State* L);
    int LuaAddBlackList(Lua_State* L);
    int LuaDelBlackList(Lua_State* L);
#ifdef _SERVER
    int LuaAddFellowshipAttraction(Lua_State* L);
    int LuaGetFellowshipAttraction(Lua_State* L);
    int LuaGetFellowshipAttractionLevel(Lua_State* L);
    int LuaIsFoe(Lua_State* L);
#endif

#ifdef _CLIENT
    int LuaSetFellowshipGroup(Lua_State* L);

    int LuaUpdateFellowshipInfo(Lua_State* L);
    int LuaUpdateFoeInfo(Lua_State* L);
    int LuaUpdateBlackListInfo(Lua_State* L);

    int LuaGetFellowshipGroupName(Lua_State* L);
    int LuaGetFellowshipData(Lua_State* L);
    int LuaGetFellowshipInfo(Lua_State* L);
    int LuaGetFoeInfo(Lua_State* L);
    int LuaGetBlackListInfo(Lua_State* L);
    int LuaGetFellowshipGroupInfo(Lua_State* L);
    int LuaGetFellowshipNameList(Lua_State* L);
    int LuaIsExistFellowshipList(Lua_State* L);
#endif
    //-----------------------------------------


#ifdef _SERVER
    int LuaGetPartyMemberList(Lua_State* L);

	int LuaMapTest(Lua_State* L);
	int LuaGetSelectCharacter(Lua_State* L);
    int LuaGetSelectDoodad(Lua_State* L);
	int LuaAddMapMark(Lua_State* L);	//С��ͼ���,��·������
	int LuaRevive(Lua_State* L);
	int LuaPlaySound(Lua_State* L);
	int LuaPlayMiniGame(Lua_State* L);
	int LuaSendMessageNotify(Lua_State* L);
#endif
	int LuaSyncMidMapMark(Lua_State* L);	//�е�ͼ���,ͬ������Լ��ڵ�ͼ�����ı��

	//------------- PK ----------------------->
	int LuaCanApplyDuel(Lua_State* L);
	int LuaCanApplySlay(Lua_State* L);

	int LuaApplyDuel(Lua_State* L);
	int LuaRefuseDuel(Lua_State* L);
	int LuaAcceptDuel(Lua_State* L);
	int LuaLossDuel(Lua_State* L);
	
	int LuaApplySlay(Lua_State* L);
    int LuaCanCloseSlay(Lua_State* L);
	int LuaCloseSlay(Lua_State* L);

	int LuaGetPKState(Lua_State* L);
	//<----------------------------------------

#ifdef _SERVER
    int LuaRideHorse(Lua_State* L);
    int LuaDownHorse(Lua_State* L);

    int LuaAddEnchant(Lua_State* L);
    int LuaRemoveEnchant(Lua_State* L);
	int LuaGetEnchantDestItemSubType(Lua_State* L);

    int LuaSetEquipColor(Lua_State* L);
#endif
    
    //------------- ��ͨ ----------------------->
#ifdef _SERVER
    int LuaOpenRouteNode(Lua_State* L);
#endif
    int LuaIsOpenRouteNode(Lua_State* L);
	//<------------------------------------------

    // ------------- ��Ϊ��� -------------------->
    int LuaAddTrain(Lua_State* L);
    int LuaAddTrainNoLimit(Lua_State* L);
    int LuaSetMaxTrain(Lua_State* L);
    // <-------------------------------------------

    // ----------------- �ɾ���� ---------------->
    int LuaIsAchievementAcquired(Lua_State* L);
#ifdef _SERVER
    int LuaAcquireAchievement(Lua_State* L);
#endif
    int LuaGetAchievementPoint(Lua_State* L);
    int LuaGetAchievementRecord(Lua_State* L);
    // <-------------------------------------------

#ifdef _CLIENT
    // ------------- ���� ----------------------->
    int LuaOpenVenation(Lua_State* L);
    // <------------------------------------------
#endif

    int LuaSetTargetAStar(Lua_State* L);
    int LuaGetAroundPlayerID(Lua_State* L);

    int LuaIsOnSlay(Lua_State* L);

    int LuaSetMapVisitFlag(Lua_State* L);
    int LuaGetMapVisitFlag(Lua_State* L);

#ifdef _CLIENT
    int LuaGetReviveLeftFrame(Lua_State* L);
    int LuaIsPartyMemberInSameScene(Lua_State* L);
	int LuaOpen(Lua_State* L);
    int LuaGetQuestDiffcultyLevel(Lua_State* L);
#endif

#ifdef _SERVER
    int LuaKickout(Lua_State* L);
    int LuaBan(Lua_State* L);
    int LuaGetLastKillPointReduceTime(Lua_State* L);
    int LuaSetLastKillPointReduceTime(Lua_State* L);
    int LuaSetCurrentKillPoint(Lua_State* L);
    int LuaSetCannotDialogWithNPC(Lua_State* L);
    int LuaSetRedName(Lua_State* L);
    int LuaSetOnlyReviveInSitu(Lua_State* L);
    
    int LuaAddPrestige(Lua_State* L);

    int LuaAddPQ(Lua_State* L);
    int LuaDelPQ(Lua_State* L);
    int LuaGetPQIDTable(Lua_State* L);
    int LuaSetHeroMapCopyOpenFlag(Lua_State* L);
    int LuaGetHeroMapCopyOpenFlag(Lua_State* L);
#endif

    int LuaSetCamp(Lua_State* L);

#ifdef _SERVER
    int LuaGetReviveCtrl(Lua_State* L);
    int LuaSetReviveCtrl(Lua_State* L);
    int LuaSetMaxLevel(Lua_State* L);
#endif

#ifdef _CLIENT
    void LuaHideHat(Lua_State* L);
#endif
};
#endif	//_KPLAYER_H_