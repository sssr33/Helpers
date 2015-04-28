#include "pch.h"
#include "VectorIBuffer.h"

VectorIBuffer::~VectorIBuffer(){

}

STDMETHODIMP VectorIBuffer::RuntimeClassInitialize(){
	this->usedSize = 0;
	return S_OK;
}

STDMETHODIMP VectorIBuffer::Buffer(byte **value){
	*value = this->buffer.data();
	return S_OK;
}

STDMETHODIMP VectorIBuffer::get_Capacity(UINT32 *value){
	*value = static_cast<UINT32>(this->buffer.size());
	return S_OK;
}

STDMETHODIMP VectorIBuffer::get_Length(UINT32 *value){
	*value = this->usedSize;
	return S_OK;
}

STDMETHODIMP VectorIBuffer::put_Length(UINT32 value){
	this->usedSize = value;
	return S_OK;
}

uint32_t VectorIBuffer::GetUsedSize() const{
	return this->usedSize;
}

size_t VectorIBuffer::GetSize() const{
	return this->buffer.size();
}

void VectorIBuffer::SetSize(size_t v){
	this->buffer.resize(v);
}

const std::vector<uint8_t> &VectorIBuffer::GetBuffer() const{
	return this->buffer;
}

std::vector<uint8_t> VectorIBuffer::MoveBuffer() const{
	return std::move(this->buffer);
}

void VectorIBuffer::SetBuffer(const std::vector<uint8_t> &v){
	this->buffer = v;
}

void VectorIBuffer::SetBuffer(std::vector<uint8_t> &&v){
	this->buffer = std::move(v);
}