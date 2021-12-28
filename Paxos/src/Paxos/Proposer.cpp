#include "Proposer.h"

namespace paxos
{

Proposer::Proposer()
{
	SetPlayerCount(0, 0);
}

Proposer::Proposer(short proposerCount, short acceptorCount)
{
	SetPlayerCount(proposerCount, acceptorCount);
}

Proposer::~Proposer()
{
}

// 设置参与者数量
void Proposer::SetPlayerCount(short proposerCount, short acceptorCount)
{
	m_proposerCount = proposerCount;
	m_acceptorCount = acceptorCount;

	return;
}

// 开始propose阶段
void Proposer::StartPropose(PROPOSAL &value)
{
	m_value = value;
	m_proposeFinished = false;
	m_isAgree = false;
	m_maxAcceptedSerialNum = 0;
	m_okCount = 0;
	m_refuseCount = 0;
	m_start = time(NULL);

	return;
}

// 取得提议
PROPOSAL& Proposer::GetProposal()
{
	return m_value;
}

//提议被投票，Proposed失败则重新开始Propose阶段
bool Proposer::Proposed(bool ok, PROPOSAL &lastAcceptValue)
{
	if ( m_proposeFinished ) return true;//可能是一阶段迟到的回应，直接忽略消息

	if ( !ok ) // 提议被拒绝
	{
		m_refuseCount++;
		//已有半数拒绝，不需要等待其它acceptor投票了，重新开始Propose阶段
		//使用StartPropose(m_value)重置状态
	
        //请完善下面逻辑
 		/**********Begin**********/
		if (m_refuseCount > m_acceptorCount / 2) {
			m_value.serialNum += num_proposal_inc;
			StartPropose(m_value);
			return false;
		}
	    /**********End**********/

		//拒绝数不到一半
		return true;
	}

	m_okCount++;
	/*
		没有必要检查分支：serialNum为null
		因为serialNum>m_maxAcceptedSerialNum，与serialNum非0互为必要条件
	*/
	//如果已经有提议被接受，修改成已被接受的提议
	//请完善下面逻辑
 	/**********Begin**********/
	if (lastAcceptValue.serialNum > m_maxAcceptedSerialNum) {
		m_maxAcceptedSerialNum = lastAcceptValue.serialNum;
		m_value.value = lastAcceptValue.value;
		return true;
	}
	/**********End**********/	

	//如果自己的提议被接受
	if ( m_okCount > m_acceptorCount / 2 ) 
	{
		m_okCount = 0;
		m_proposeFinished = true;
	}
	return true;
}

//开始Accept阶段,满足条件成功开始accept阶段返回ture，不满足开始条件返回false
bool Proposer::StartAccept()
{
	return m_proposeFinished;
}

//提议被接受，Accepted失败则重新开始Propose阶段
bool Proposer::Accepted(bool ok)
{
	if ( !m_proposeFinished ) return true;//可能是上次第二阶段迟到的回应，直接忽略消息

	if ( !ok ) 
	{
		m_refuseCount++;
		//已有半数拒绝，不需要等待其它acceptor投票了，重新开始Propose阶段
		//使用StartPropose(m_value)重置状态
	    //请完善下面逻辑
        /**********Begin**********/
		if (m_refuseCount > m_acceptorCount / 2) {
			m_value.serialNum += num_proposal_inc;
			StartPropose(GetProposal());
			return false;
		}
        /**********End**********/
 		
		return true;
	}

	m_okCount++;
	if ( m_okCount > m_acceptorCount / 2 ) m_isAgree = true;

	return true;
}

// 提议被批准
bool Proposer::IsAgree()
{
	return m_isAgree;
}

}
