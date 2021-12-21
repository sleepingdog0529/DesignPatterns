/*********************************************************************
 * @file   pimpl.h
 * @author Tomomi Murakami
 * @date   2021/12/17 21:46
 * @brief  PImplイディオム
 ********************************************************************/
#pragma once
#include <memory>

class Hoge
{
public:
	explicit Hoge(const int v);
	~Hoge();

	int GetValue() const;

private:
	class Impl;
	std::unique_ptr<Impl> pimpl_;
};