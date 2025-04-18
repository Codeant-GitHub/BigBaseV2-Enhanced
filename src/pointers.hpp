#pragma once
#include "function_types.hpp"
#include "gta/fwddec.hpp"
#include "gta_pointers.hpp"
#include "memory/batch.hpp"
#include "memory/byte_patch.hpp"
#include "memory/module.hpp"
#include "sc_pointers.hpp"
#include "util/compile_time_helpers.hpp"

namespace big
{
	class pointers
	{
	private:
		static void get_gta_batch(memory::module region);
		static void get_sc_batch(memory::module region);

	public:
		explicit pointers();
		~pointers();

	public:
		gta_pointers m_gta;

#ifdef ENABLE_SOCIALCLUB
		socialclub_pointers m_sc;
#endif // ENABLE_SOCIALCLUB		
	};

	inline pointers* g_pointers{};
}