#pragma once

#include <wrl.h>
#include <wrl/implements.h>
#include <windows.storage.streams.h>
#include <robuffer.h>
#include <vector>
#include <cstdint>

class VectorIBuffer : 
	public Microsoft::WRL::RuntimeClass < Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::RuntimeClassType::WinRtClassicComMix>,
	ABI::Windows::Storage::Streams::IBuffer,
	Windows::Storage::Streams::IBufferByteAccess >
{
public:
	virtual ~VectorIBuffer();

	STDMETHOD(RuntimeClassInitialize());

	STDMETHOD(Buffer(byte **value));

	STDMETHOD(get_Capacity(UINT32 *value));

	STDMETHOD(get_Length(UINT32 *value));
	STDMETHOD(put_Length(UINT32 value));

	uint32_t GetUsedSize() const;

	size_t GetSize() const;
	void SetSize(size_t v);

	const std::vector<uint8_t> &GetBuffer() const;
	std::vector<uint8_t> MoveBuffer() const;

	void SetBuffer(const std::vector<uint8_t> &v);
	void SetBuffer(std::vector<uint8_t> &&v);
private:
	std::vector<uint8_t> buffer;
	uint32_t usedSize;
};