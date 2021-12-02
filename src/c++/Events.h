#pragma once

namespace Events
{
	class MenuOpenCloseEventHandler :
		public RE::BSTEventSink<RE::MenuOpenCloseEvent>
	{
	public:
		~MenuOpenCloseEventHandler()
		{
			std::memset(std::addressof(_bethImpl), 0, sizeof(_bethImpl));
		}

		static inline MenuOpenCloseEventHandler* GetSingleton()
		{
			static MenuOpenCloseEventHandler singleton;
			return &singleton;
		}

		virtual RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override
		{
			auto InterfaceStrings = RE::InterfaceStrings::GetSingleton();
			if (a_event && a_event->opening && a_event->menuName == InterfaceStrings->journalMenu) {
				auto UI = RE::UI::GetSingleton();
				auto JournalMenu = UI->GetMenu(InterfaceStrings->journalMenu);
				if (JournalMenu && JournalMenu->uiMovie) {
					JournalMenu->uiMovie->SetState(RE::GFxState::StateType::kTranslator, _bethImpl.get());
				}
			}

			return RE::BSEventNotifyControl::kContinue;
		}

		inline void SetBethTranslator(RE::GPtr<RE::GFxTranslator>&& a_translator)
		{
			_bethImpl = std::move(a_translator);
		}

	private:
		RE::GPtr<RE::GFxTranslator> _bethImpl;
	};
}
