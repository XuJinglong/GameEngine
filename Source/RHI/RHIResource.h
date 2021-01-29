#pragma once
class RHIResourceRef
{
public:
	RHIResourceRef() {}
	RHIResourceRef(void* pRes) : m_pRHIRes(pRes) {}
	~RHIResourceRef() {}

	template<class T>
	static RHIResourceRef* Create(T* pRHIRes)
	{
		return new RHIResourceRef(pRHIRes);
	}

	template<class T>
	T* Get() 
	{
			return reinterpret_cast<T*>(m_pRHIRes);
	}

	bool IsValid() { return m_pRHIRes != nullptr; }
private:
	void* m_pRHIRes = nullptr;
};

