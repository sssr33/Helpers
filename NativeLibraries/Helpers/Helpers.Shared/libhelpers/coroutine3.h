#pragma once

#include <csetjmp>
#include <vector>
#include <cstdint>
#include <memory>

#define CO3_BEGIN(co, _st_type) CoroGuard3 _coGuard(&co); auto st = co.AllocStack<_st_type>(); co.Load();
#define CO3_IN(co, _st_type, fn) setjmp(co.GetInfo()->jmp); co.GetInfo()->jmpValid = true; if (fn){ _coGuard.CoroReturn(); return true; }
#define CO3_OUT(co, _st_type) setjmp(co.GetInfo()->jmp); co.GetInfo()->jmpValid = true; if(!co.GetLoaded()){ _coGuard.CoroReturn(); return true; } co.ResetLoaded();
//#define CO3_END(co) co.FreeStack(); return false;
#define CO3_END(co) return false;

class CoroStackInfo3{
public:
	uint32_t offset;
	std::vector<uint8_t> *memory;
	bool jmpValid;
	jmp_buf jmp;

	CoroStackInfo3()
		: offset(-1), jmpValid(false){
	}
	virtual ~CoroStackInfo3(){
	}

	template<class T>
	T *GetStackTypedPtr(){
		T *typedPtr = nullptr;

		if (this->memory && this->offset != static_cast<uint32_t>(-1)){
			typedPtr = reinterpret_cast<T *>(&(*this->memory)[this->offset]);
		}

		return typedPtr;
	}
};

template<class T>
class GenericCoroStackInfo3 : public CoroStackInfo3{
	virtual ~GenericCoroStackInfo3(){
		auto typedPtr = this->GetStackTypedPtr<T>();
		if (typedPtr){
			typedPtr->~T();
		}
	}
};

class coroutine3{
public:
	coroutine3()
		: stackLevel(-1), loaded(false){
	}

	~coroutine3(){
		this->varStackInfo.clear();
		this->varStack.clear();
	}

	bool GetLoaded() const{
		return this->loaded;
	}

	void ResetLoaded(){
		this->loaded = false;
	}

	std::unique_ptr<CoroStackInfo3> &GetInfo(){
		return this->varStackInfo[this->stackLevel];
	}

	template<class T>
	T *AllocStack(){
		T *st;
		this->stackLevel++;

		if (this->stackLevel >= this->varStackInfo.size()){
			std::unique_ptr<CoroStackInfo3> stInfo = std::unique_ptr<CoroStackInfo3>(new GenericCoroStackInfo3<T>);
			stInfo->offset = this->varStack.size();
			stInfo->memory = &this->varStack;

			this->varStackInfo.push_back(std::move(stInfo));
			this->varStack.insert(this->varStack.end(), sizeof(T), 0);

			st = this->varStackInfo[this->stackLevel]->GetStackTypedPtr<T>();
			new (st)T();
		}

		st = this->varStackInfo[this->stackLevel]->GetStackTypedPtr<T>();
		return st;
	}

	void FreeStack(){
		if (this->stackLevel < this->varStackInfo.size()){
			auto offset = this->varStackInfo[this->stackLevel]->offset;
			auto popCount = this->varStack.size() - offset;

			this->varStackInfo.pop_back();
			this->varStack._Pop_back_n(popCount);
		}

		this->stackLevel--;
	}

	void Load(){
		auto &info = this->varStackInfo[this->stackLevel];
		if (info->jmpValid){
			this->loaded = true;
			longjmp(info->jmp, 0);
		}
	}

	void LevelUp(){
		this->stackLevel--;
	}
private:
	std::vector<uint8_t> varStack;
	std::vector<std::unique_ptr<CoroStackInfo3>> varStackInfo;
	uint32_t stackLevel;
	bool loaded;
};

class CoroGuard3{
public:
	coroutine3 *coro;

	CoroGuard3()
		: coro(nullptr){
	}

	CoroGuard3(coroutine3 *coro)
		: coro(coro){
	}

	~CoroGuard3(){
		if (coro){
			coro->FreeStack();
		}
	}

	void CoroReturn(){
		if (coro){
			coro->LevelUp();
			coro = nullptr;
		}
	}
};