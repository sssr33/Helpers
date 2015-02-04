#include "pch.h"
#include "Class1.h"
#include "Folder/FolderClass.h"
#include "Folder2/FolderClass.h"
#include "containers\chunked_data_buffer.h"
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
	auto res = Folder1::FolderClass::Method(1, 2);
	auto res2 = Folder2::FolderClass::Method(1, 2);

	service_TEST();

	int stop = 432;
}

void chunked_data_buffer_TEST(){
	size_t sz = 0;
	uint8_t data[] = { 1, 2, 3, 4, 5, 6, 7 };
	uint8_t dataCopy[6];
	chunked_data_buffer<uint8_t, 3> buffer;

	buffer.push_back(1);
	sz = buffer.size();

	buffer.pop_front(1);
	sz = buffer.size();

	buffer.push_back(1);
	sz = buffer.size();

	buffer.pop_front(1);
	sz = buffer.size();

	buffer.push_back(data, data + sizeof(data));
	sz = buffer.size();

	buffer.pop_front();
	sz = buffer.size();

	buffer.pop_front();
	sz = buffer.size();

	auto v = buffer.front();
	auto copied = buffer.front(dataCopy, dataCopy + sizeof(dataCopy));

	buffer.pop_front(2);
	sz = buffer.size();

	buffer.pop_front(3);
	sz = buffer.size();
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