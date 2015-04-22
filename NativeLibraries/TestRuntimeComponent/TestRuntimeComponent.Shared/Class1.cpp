#include "pch.h"
#include "Class1.h"
#include <libhelpers\Containers\ChunkedDataBuffer.h>
#include <libhelpers\service\Operation.h>
#include <libhelpers\service\Service.h>
#include <libhelpers\Thread\ThreadPool.h>

#include <queue>
#include <memory>

using namespace TestRuntimeComponent;
using namespace Platform;

void chunked_data_buffer_TEST();
void service_TEST();
void ThreadPool_TEST();

Class1::Class1(){
}

void Class1::Test(){
	//service_TEST();
	ThreadPool_TEST();
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
class MapFactoryHelper < int, float > {
public:
	typedef std::map<int, float> Res;
};

// <void, float> spec. have no sense
template<>
class MapFactoryHelper < float, void > {
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

	int stop = 32;
}