/*********************************************************************
 * @file   event_dispatcher.h
 * @author Tomomi Murakami
 * @date   2021/12/12 15:10
 * @brief  イベント発行機とイベントリスナー
 ********************************************************************/
#pragma once
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <unordered_set>

namespace wx2
{
	// 前方宣言
	template <typename EventType, typename... Args>
	class EventDispatcher;

	/**
	 * @brief  イベントリスナー
	 * @tparam EventType イベントの種類を判別するためのキーの型
	 * @tparam Args コールバックに渡される引数の型
	 */
	template <typename EventType, typename... Args>
	class EventListener final
	{
		//! 対応したイベント発行クラスの型
		using EventDispatcherType = EventDispatcher<EventType, Args...>;
		//! コールバック関数型
		using CallbackFunc = std::function<void(Args...)>;

		//! イベント発行クラスのアクセスを許可
		friend class EventDispatcherType;

	public:
		EventListener() = default;
		/**
		 * @brief コールバック関数をセットする
		 * @param callback コールバック関数
		 */
		EventListener(CallbackFunc callback)
			: callback_(std::move(callback)) {}
		~EventListener() = default;

		// ムーブとコピーを許可
		EventListener(const EventListener& other) = default;
		EventListener& operator=(const EventListener& other) = default;
		EventListener(EventListener&& other) = default;
		EventListener& operator=(EventListener&& other) = default;

		/**
		 * @brief コールバック関数をセットする
		 * @param callback コールバック関数
		 */
		void SetCallback(const CallbackFunc& callback) noexcept
		{
			callback_ = callback;
		}

		/**
		 * @brief コールバックの登録を解除する
		 */
		void Clear() noexcept
		{
			// 自身が登録されている全てのイベント発行クラスから自身を登録解除する
			for (auto& dispatcher : eventDispatchers_)
			{
				if (dispatcher)
				{
					dispatcher->RemoveEventListener(*this);
				}
			}
		}

		/**
		 * @brief  コールバック関数を呼び出し可能か調べる
		 * @return 呼び出し可能か
		 */
		[[nodiscard]] bool HasCallback() const noexcept { return !!callback_; }

	private:
		/**
		 * @brief  コールバックを呼び出す
		 * @param  args コールバックの引数
		 */
		void OnDispatch(const Args&... args)
		{
			callback_(args...);
		}

		//! コールバック関数
		CallbackFunc callback_{};
		//! 自身が登録されている全てのイベント発行クラス
		std::unordered_set<EventDispatcherType*> eventDispatchers_{};
	};

	/**
	 * @brief  イベント発行機
	 * @tparam EventType イベントの種類を判定するためのキーの型
	 * @tparam Args コールバックに渡される引数の型
	 */
	template <typename EventType, typename... Args>
	class EventDispatcher
	{
		//! イベントリスナーの型
		using EventListenerType = EventListener<EventType, Args...>;

	public:
		EventDispatcher() = default;
		virtual ~EventDispatcher() = default;

		// ムーブとコピーを許可
		EventDispatcher(const EventDispatcher& other) = default;
		EventDispatcher& operator=(const EventDispatcher& other) = default;
		EventDispatcher(EventDispatcher&& other) = default;
		EventDispatcher& operator=(EventDispatcher&& other) = default;

		/**
		 * @brief イベントを発行する
		 * @param event イベントの種類
		 * @param args コールバックに渡す引数
		 */
		void Dispatch(const EventType& event, const Args&... args)
		{
			auto [itr, end] = listeners_.equal_range(event);
			for (; itr != end; ++itr)
			{
				itr->second->OnDispatch(args...);
			}
		}

		/**
		 * @brief イベントリスナーを登録する
		 * @param event イベントの種類
		 * @param eventListener 登録するイベントリスナー
		 */
		void AddEventListener(EventType&& event, EventListenerType& eventListener)
		{
			eventListener.eventDispatchers_.insert(this);
			listeners_.emplace(std::forward<EventType>(event), &eventListener);
		}

		/**
		 * @brief イベントリスナーを除外する
		 * @param event 除外するイベントの種類
		 * @param eventListener 除外するイベントリスナー
		 */
		void RemoveEventListener(const EventType& event, EventListenerType& eventListener)
		{
			std::erase_if(
				listeners_,
				[&](const auto& l) { return l.first == event && l.second == &eventListener; });
			eventListener.eventDispatchers_.erase(this);
		}

	private:
		//! イベントリスナーの連想配列
		std::unordered_multimap<EventType, EventListenerType*> listeners_{};
	};
}