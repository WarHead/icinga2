/******************************************************************************
 * Icinga 2                                                                   *
 * Copyright (C) 2012-2018 Icinga Development Team (https://www.icinga.com/)  *
 *                                                                            *
 * This program is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU General Public License                *
 * as published by the Free Software Foundation; either version 2             *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software Foundation     *
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ******************************************************************************/

#ifndef FUNCTION_H
#define FUNCTION_H

#include "base/i2-base.hpp"
#include "base/function-ti.hpp"
#include "base/value.hpp"
#include "base/functionwrapper.hpp"
#include "base/scriptglobal.hpp"
#include <vector>

namespace icinga
{

/**
 * A script function that can be used to execute a script task.
 *
 * @ingroup base
 */
class Function final : public ObjectImpl<Function>
{
public:
	DECLARE_OBJECT(Function);

	typedef std::function<Value (const std::vector<Value>& arguments)> Callback;

	template<typename F>
	Function(const String& name, F function, const std::vector<String>& args = std::vector<String>(),
		bool side_effect_free = false, bool deprecated = false)
		: Function(name, WrapFunction(function), args, side_effect_free, deprecated)
	{ }

	Value Invoke(const std::vector<Value>& arguments = std::vector<Value>());
	Value InvokeThis(const Value& otherThis, const std::vector<Value>& arguments = std::vector<Value>());

	bool IsSideEffectFree() const
	{
		return GetSideEffectFree();
	}

	bool IsDeprecated() const
	{
		return GetDeprecated();
	}

	static Object::Ptr GetPrototype();

	Object::Ptr Clone() const override;

private:
	Callback m_Callback;

	Function(const String& name, Callback function, const std::vector<String>& args,
		bool side_effect_free, bool deprecated);
};

#define REGISTER_FUNCTION(ns, name, callback, args) \
	INITIALIZE_ONCE_WITH_PRIORITY([]() { \
		Function::Ptr sf = new icinga::Function(#ns "#" #name, callback, String(args).Split(":"), false); \
		Namespace::Ptr nsp = ScriptGlobal::Get(#ns); \
		nsp->SetAttribute(#name, std::make_shared<ConstEmbeddedNamespaceValue>(sf)); \
	}, 10)

#define REGISTER_SAFE_FUNCTION(ns, name, callback, args) \
	INITIALIZE_ONCE_WITH_PRIORITY([]() { \
		Function::Ptr sf = new icinga::Function(#ns "#" #name, callback, String(args).Split(":"), true); \
		Namespace::Ptr nsp = ScriptGlobal::Get(#ns); \
		nsp->SetAttribute(#name, std::make_shared<ConstEmbeddedNamespaceValue>(sf)); \
	}, 10)

#define REGISTER_FUNCTION_NONCONST(ns, name, callback, args) \
	INITIALIZE_ONCE_WITH_PRIORITY([]() { \
		Function::Ptr sf = new icinga::Function(#ns "#" #name, callback, String(args).Split(":"), false); \
		Namespace::Ptr nsp = ScriptGlobal::Get(#ns); \
		nsp->SetAttribute(#name, std::make_shared<EmbeddedNamespaceValue>(sf)); \
	}, 10)

#define REGISTER_SAFE_FUNCTION_NONCONST(ns, name, callback, args) \
	INITIALIZE_ONCE_WITH_PRIORITY([]() { \
		Function::Ptr sf = new icinga::Function(#ns "#" #name, callback, String(args).Split(":"), true); \
		Namespace::Ptr nsp = ScriptGlobal::Get(#ns); \
		nsp->SetAttribute(#name, std::make_shared<EmbeddedNamespaceValue>(sf)); \
	}, 10)

}

#endif /* FUNCTION_H */
