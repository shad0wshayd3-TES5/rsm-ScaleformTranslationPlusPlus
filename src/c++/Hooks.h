#pragma once

#include "Events.h"
#include "LocaleManager.h"

namespace Hooks
{
	namespace
	{
		class BSScaleformMovieLoaderEx :
			public RE::BSScaleformManager
		{
		public:
			BSScaleformMovieLoaderEx* Ctor()
			{
				_ctor(this);

				auto BSScaleformTranslator = loader->GetState<RE::BSScaleformTranslator>(RE::GFxState::StateType::kTranslator);

				auto newTranslator = new LocaleManager(BSScaleformTranslator->translator);
				loader->SetState(RE::GFxState::StateType::kTranslator, newTranslator);

				auto menuSink = Events::MenuOpenCloseEventHandler::GetSingleton();
				menuSink->SetBethTranslator(std::move(BSScaleformTranslator));

				return this;
			}

			static void InstallHooks()
			{
				REL::Relocation<std::uintptr_t> target(REL::ID(36547), 0xDA1);
				auto& trampoline = SKSE::GetTrampoline();
				_ctor = trampoline.write_call<5>(target.address(), &BSScaleformMovieLoaderEx::Ctor);

				logger::info("Installed hooks for (%s)", typeid(BSScaleformMovieLoaderEx).name());
			}

		private:
			static inline REL::Relocation<decltype(&BSScaleformMovieLoaderEx::Ctor)> _ctor;
		};
	}

	void InstallHooks()
	{
		BSScaleformMovieLoaderEx::InstallHooks();
		logger::info("Installed all hooks");
	}
}
