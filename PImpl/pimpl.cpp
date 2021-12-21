#include "pimpl.h"
#include <cassert>

class Hoge::Impl
{
public:
	explicit Impl(const int v)
		: val_(v) {}

	int GetValue() const
	{
		return val_;
	}

private:
	int val_;
};

Hoge::Hoge(const int v)
	: pimpl_(std::make_unique<Impl>(v)) {}

Hoge::~Hoge() = default;

int Hoge::GetValue() const
{
	return pimpl_->GetValue();
}

int main()
{
	// Hoge‚Ìval_‚ª‰B•Á‚Å‚«‚é
	const Hoge hoge(5);
	assert(hoge.GetValue() == 5);

	return 0;
}