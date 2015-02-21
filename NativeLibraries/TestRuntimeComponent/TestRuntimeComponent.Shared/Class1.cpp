#include "pch.h"
#include "Class1.h"
#include "Containers\ChunkedDataBuffer.h"
#include "service\Operation.h"
#include "service\Service.h"

#include <queue>
#include <memory>

using namespace TestRuntimeComponent;
using namespace Platform;

void chunked_data_buffer_TEST();
void service_TEST();

Class1::Class1(){
}

void Class1::Test(){
	service_TEST();
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