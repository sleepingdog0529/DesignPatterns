/*********************************************************************
 * @file   event_dispatcher.h
 * @author Tomomi Murakami
 * @date   2021/12/12 15:10
 * @brief  イベントディスパッチャーパターン
 ********************************************************************/
#pragma once
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <unordered_set>

// 前方宣言
template <typename EventType, typename... Args>
class EventDispatcher;

/**
 * @brief  イベントリスナー
 * @tparam EventType イベントの種類を判別するためのキーの型
 * @tparam CallbackArgsType コールバックに渡される引数の型
 */
template <typename EventType, typename... CallbackArgsType>
class EventListener final
{
	//! 対応したイベント発行クラスの型
	using EventDispatcherType = EventDispatcher<EventType, CallbackArgsType...>;
	//! コールバック関数型
	using CallbackFunc = std::function<void(CallbackArgsType ...)>;

	//! イベント発行クラスのアクセスを許可
	friend class EventDispatcherType;

public:
	EventListener() = default;
	/**
	 * @brief コールバック関数をセットする
	 * @param callback コールバック関数
	 */
	explicit EventListener(CallbackFunc callback) noexcept
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
	 * @brief 初期化する
	 */
	void Clear() noexcept
	{
		callback_ = {};
	}

	/**
	 * @brief  コールバック関数を呼び出し可能か調べる
	 * @return 呼び出し可能か
	 */
	[[nodiscard]] bool HasCallback() const noexcept
	{
		return static_cast<bool>(callback_);
	}

private:
	/**
	 * @brief コールバックを呼び出す
	 * @param args コールバックの引数
	 */
	void OnDispatch(const CallbackArgsType&... args) const
	{
		if (HasCallback())
		{
			callback_(args...);
		}
	}
	
	//! コールバック関数
	CallbackFunc callback_{};
};

/**
 * @brief  イベント発行機
 * @tparam EventType イベントの種類を判定するためのキーの型
 * @tparam CallbackArgsType コールバックに渡される引数の型
 */
template <typename EventType, typename... CallbackArgsType>
class EventDispatcher
{
	//! イベントリスナーの型
	using EventListenerType = EventListener<EventType, CallbackArgsType...>;

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
	void Dispatch(EventType&& event, const CallbackArgsType&... args)
	{
		auto [itr, end] = listeners_.equal_range(std::forward<EventType>(event));
		for (; itr != end; ++itr)
		{
			if (itr->second)
				itr->second->OnDispatch(args...);
			else
				itr = listeners_.erase(itr);
		}
	}

	/**
	 * @brief イベントリスナーを登録する
	 * @param event イベントの種類
	 * @param eventListener 登録するイベントリスナー
	 */
	void AppendListener(EventType&& event, EventListenerType& eventListener)
	{
		listeners_.emplace(std::forward<EventType>(event), &eventListener);
	}

	/**
	 * @brief 指定したイベントのイベントリスナーを登録解除する
	 * @param event 登録解除するイベントの種類
	 * @param eventListener 登録解除するイベントリスナー
	 */
	void RemoveListener(const EventType& event, EventListenerType& eventListener)
	{
		std::erase_if(
			listeners_,
			[&](const auto& x) { return x.first == event && x.second == &eventListener; });
	}

	/**
	 * @brief イベントリスナーを全て登録解除
	 */
	void Clear()
	{
		listeners_.clear();
	}

private:
	//! イベントリスナーの連想配列
	std::unordered_multimap<EventType, EventListenerType*> listeners_{};
};
