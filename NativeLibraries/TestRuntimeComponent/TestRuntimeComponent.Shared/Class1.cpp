#include "pch.h"
#include "Class1.h"
#include <libhelpers\Containers\ChunkedDataBuffer.h>
#include <libhelpers\service\Operation.h>
#include <libhelpers\service\Service.h>
#include <libhelpers\Thread\ThreadPool.h>
#include <libhelpers\Containers\ConcurrentQueue.h>
#include <libhelpers\Thread\PPL\critical_section_guard.h>
#include <libhelpers\H.h>
#include <libhelpers\Containers\dynamic_stack_array.h>

#include <queue>
#include <memory>

using namespace TestRuntimeComponent;
using namespace Platform;

void chunked_data_buffer_TEST();
void service_TEST();
void ThreadPool_TEST();

Class1::Class1(){
}

struct ttt{
	int v;
};

void Class1::Test(){

	{
		dynamic_stack_array<size_t> tmp2(10);

		tmp2[0] = 123;

		auto v = tmp2[0];

		dynamic_stack_array<size_t> tmp3, tmp4;

		tmp3 = tmp2;
		tmp4 = std::move(tmp2);

		int stop = 243;
	}
	



	ttt tmp[4];

	tmp[0].v = 12;
	tmp[1].v = 34;
	tmp[2].v = 56;
	tmp[3].v = 78;

	critical_section_guard<ttt> guard_value(tmp[0]);
	critical_section_guard<ttt *> guard_pointer(&tmp[1]);
	critical_section_guard<std::unique_ptr<ttt>> guard_u_pointer(std::unique_ptr<ttt>(new ttt(tmp[2])));
	critical_section_guard<std::shared_ptr<ttt>> guard_s_pointer(std::shared_ptr<ttt>(new ttt(tmp[3])));

	{
		auto v_tmp = guard_value.Get();
		
		v_tmp->v = 1212;
	}

	{
		auto p_tmp = guard_pointer.Get();

		p_tmp->v = 3434;
	}

	{
		auto up_tmp = guard_u_pointer.Get();

		up_tmp->v = 5656;
	}

	{
		auto sp_tmp = guard_s_pointer.Get();

		sp_tmp->v = 7878;
	}

	//service_TEST();
	//ThreadPool_TEST();
}

void chunked_data_buffer_TEST(){
	size_t sz = 0;
	uint8_t data[] = { 1, 2, 3, 4, 5, 6, 7 };
	uint8_t dataCopy[6];
	ChunkedDataBuffer<uint8_t, 3> buffer;

	buffer.PushBack(1);
	sz = buffer.Size();

	buffer.PopFront(1);
	sz = buffer.Size();

	buffer.PushBack(1);
	sz = buffer.Size();

	buffer.PopFront(1);
	sz = buffer.Size();

	buffer.PushBack(data, data + sizeof(data));
	sz = buffer.Size();

	buffer.PopFront();
	sz = buffer.Size();

	buffer.PopFront();
	sz = buffer.Size();

	auto v = buffer.Front();
	auto copied = buffer.Front(dataCopy, dataCopy + sizeof(dataCopy));

	buffer.PopFront(2);
	sz = buffer.Size();

	buffer.PopFront(3);
	sz = buffer.Size();
}



typedef Service<int> IntServ;

class Op1 : public IntServ::OpBase < Op1, float > {
public:
	Ret RunImpl(ServiceArgs args){

		args.data = 34;

		throw std::exception("DSFSDF");

		return 3.14f;
	}
};

void service_TEST(){
	IntServ service;

	Op1 *op1 = new Op1();
	auto res = op1->GetResult();
	std::unique_ptr<IntServ::IOp> op(op1);

	service.TestRun(op.get());

	//op->Run(args);

	//auto v = res.get();
}



std::mutex testVecMtx;
std::vector<int> testVec;

void testPush(int v){
	std::lock_guard<std::mutex> lk(testVecMtx);

	if (testVec.empty()){
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	}

	testVec.push_back(v);
}



#include <type_traits>
#include <map>

template<class T>
class SigHelper{
};

template<class R, class T>
class SigHelper<R(T::*)() const>{
public:
	typedef R R;
	typedef T T;
	typedef void A1;
	typedef void A2;
};

template<class R, class T, class A1>
class SigHelper<R(T::*)(A1) const>{
public:
	typedef R R;
	typedef T T;
	typedef A1 A1;
	typedef void A2;
};

template<class R, class T, class A1, class A2>
class SigHelper<R(T::*)(A1, A2) const>{
public:
	typedef R R;
	typedef T T;
	typedef A1 A1;
	typedef A2 A2;
};


template<class T>
class Signature{
public:
	typedef typename SigHelper<decltype(&T::operator())>::R R;
	typedef typename SigHelper<decltype(&T::operator())>::A1 A1;
	typedef typename SigHelper<decltype(&T::operator())>::A2 A2;
};


template<class A1, class A2>
class MapFactoryHelper{
public:
	typedef std::map<int, int> Res;
};

template<>
class MapFactoryHelper<int, float>{
public:
	typedef std::map<int, float> Res;
};

// <void, float> spec. have no sense
template<>
class MapFactoryHelper<float, void>{
public:
	typedef std::vector<float> Res;
};

template<class T>
class MapFactory{
public:
	typedef typename MapFactoryHelper<typename Signature<T>::A1, typename Signature<T>::A2>::Res Res;
};


template<class T>
typename MapFactory<T>::Res TTT(T t){
	return MapFactory<T>::Res();
}


void ThreadPool_TEST(){
	auto tp = ThreadPool::Make();

	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	int a1 = 213;

	auto b = [&](float a, double d){ 
		a1++;
		return a1 + a + 23.23; 
	};

	std::function<double(float)> fff;

	auto res = TTT(fff);

	int stop = 32;
}