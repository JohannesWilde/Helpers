namespace Helpers
{

/**
 * @brief The AbstractState class implements the methods for initializing, processing
 * and deinitializing data for the respective State in a Statemachine.
 */
template<typename DATA_TYPE>
class AbstractState
{
public:

    virtual void init(DATA_TYPE & data) const = 0;

    virtual AbstractState const & process(DATA_TYPE & data) const = 0;

    virtual void deinit(DATA_TYPE & data) const = 0;

protected:

    constexpr AbstractState() = default;

    AbstractState(const AbstractState &) = delete;
    AbstractState(AbstractState &&) = delete;
    AbstractState &operator=(const AbstractState &) = delete;
    AbstractState &operator=(AbstractState &&) = delete;
};

/**
 * @brief The NoopState class serves as a non-operational instance.
 * It was introduced, to avoid nullptr-checks with every process()-call.
 */
template<typename DATA_TYPE>
class NoopState : public AbstractState<DATA_TYPE>
{
public:
    void init(DATA_TYPE & data) const override
    {
        // intentionally empty
    }

    AbstractState<DATA_TYPE> const & process(DATA_TYPE & data) const override
    {
        // intentionally empty
        return *this;
    }

    void deinit(DATA_TYPE & data) const override
    {
        // intentionally empty
    }
};

/**
 * @brief The Statemachine class provides a generic statemachine implementation.
 * For its usage see example below.
 */
template <typename DATA_TYPE>
class Statemachine
{
public:

    constexpr Statemachine(AbstractState<DATA_TYPE> const & startState)
        : previousState_(&noopState_)
        , currentState_(&startState)
    {
        // intentionally empty
    }

    void process(DATA_TYPE & data)
    {
        if (previousState_ != currentState_)
        {
            previousState_->deinit(data);
            currentState_->init(data);
            previousState_ = currentState_;
        }

        // Finish processing with the processing of the current state, so that
        // the result will be externally visible afterwards.
        currentState_ = &currentState_->process(data);
    }

    void reset(DATA_TYPE & data, AbstractState<DATA_TYPE> const & startState)
    {
        // if (previousState_ == currentState_) -> this is the same as calling currentState_->deinit(data);
        // if (previousState_ != currentState_) -> deinit() previousState_ and don't even init() currentState_.
        previousState_->deinit(data);
        previousState_ = &noopState_;

        currentState_ = &startState;
    }

private:

    AbstractState<DATA_TYPE> const * previousState_;
    AbstractState<DATA_TYPE> const * currentState_;

    static NoopState<DATA_TYPE> const noopState_;
};

// ODR ensures, this exists only once per DATA_TYPE.
template <typename DATA_TYPE>
NoopState<DATA_TYPE> const Statemachine<DATA_TYPE>::noopState_;


/* ----- Example -----
 *
 * #include <iostream>
 *
 * struct TrafficLight
 * {
 *     bool red    = false;
 *     bool yellow = false;
 *     bool green  = false;
 * };
 *
 * char const * lightStateToSymbol(bool const state)
 * {
 *     return state ? "X" : " ";
 * }
 *
 * std::ostream & operator<<(std::ostream & output, TrafficLight const & trafficLight)
 * {
 *     output << "|(" <<
 *         lightStateToSymbol(trafficLight.red) << ")(" <<
 *         lightStateToSymbol(trafficLight.yellow) << ")(" <<
 *         lightStateToSymbol(trafficLight.green) << ")|";
 *
 *     return output;
 * }
 *
 * struct DataType
 * {
 *     TrafficLight trafficLight;
 *     int remainingDurationInPhase = 0;
 * };
 *
 *
 *
 * class StateRed : public AbstractState<DataType>
 * {
 *     void init(DataType & data) const override;
 *
 *     AbstractState const & process(DataType & data) const override;
 *
 *     void deinit(DataType & data) const override;
 * };
 *
 * static StateRed const stateRed;
 *
 *
 * class StateRedYellow : public AbstractState<DataType>
 * {
 *     void init(DataType & data) const override;
 *
 *     AbstractState const & process(DataType & data) const override;
 *
 *     void deinit(DataType & data) const override;
 * };
 *
 * static StateRedYellow const stateRedYellow;
 *
 *
 * class StateGreen : public AbstractState<DataType>
 * {
 *     void init(DataType & data) const override;
 *
 *     AbstractState const & process(DataType & data) const override;
 *
 *     void deinit(DataType & data) const override;
 * };
 *
 * static StateGreen const stateGreen;
 *
 *
 * class StateYellow : public AbstractState<DataType>
 * {
 *     void init(DataType & data) const override;
 *
 *     AbstractState const & process(DataType & data) const override;
 *
 *     void deinit(DataType & data) const override;
 * };
 *
 * static StateYellow const stateYellow;
 *
 *
 *
 *
 * void StateRed::init(DataType & data) const
 * {
 *     std::cout << "StateRed::init" << std::endl;
 *     data.remainingDurationInPhase = 10;
 *     data.trafficLight.red = true;
 *     data.trafficLight.green = false;
 *     data.trafficLight.yellow = false;
 * }
 *
 * AbstractState<DataType> const & StateRed::process(DataType & data) const
 * {
 *     std::cout << "StateRed::process" << std::endl;
 *
 *     --data.remainingDurationInPhase;
 *     if (0 == data.remainingDurationInPhase)
 *     {
 *         return stateRedYellow;
 *     }
 *     else
 *     {
 *         return *this;
 *     }
 * }
 *
 * void StateRed::deinit(DataType & data) const
 * {
 *     std::cout << "StateRed::deinit" << std::endl;
 * }
 *
 *
 * void StateRedYellow::init(DataType & data) const
 * {
 *     std::cout << "StateRedYellow::init" << std::endl;
 *     data.remainingDurationInPhase = 2;
 *     data.trafficLight.red = true;
 *     data.trafficLight.yellow = true;
 *     data.trafficLight.green = false;
 * }
 *
 * AbstractState<DataType> const & StateRedYellow::process(DataType & data) const
 * {
 *     std::cout << "StateRedYellow::process" << std::endl;
 *
 *     --data.remainingDurationInPhase;
 *     if (0 == data.remainingDurationInPhase)
 *     {
 *         return stateGreen;
 *     }
 *     else
 *     {
 *         return *this;
 *     }
 * }
 *
 * void StateRedYellow::deinit(DataType & data) const
 * {
 *     std::cout << "StateRedYellow::deinit" << std::endl;
 * }
 *
 *
 * void StateGreen::init(DataType & data) const
 * {
 *     std::cout << "StateGreen::init" << std::endl;
 *     data.remainingDurationInPhase = 5;
 *     data.trafficLight.red = false;
 *     data.trafficLight.yellow = false;
 *     data.trafficLight.green = true;
 * }
 *
 * AbstractState<DataType> const & StateGreen::process(DataType & data) const
 * {
 *     std::cout << "StateGreen::process" << std::endl;
 *
 *     --data.remainingDurationInPhase;
 *     if (0 == data.remainingDurationInPhase)
 *     {
 *         return stateYellow;
 *     }
 *     else
 *     {
 *         return *this;
 *     }
 * }
 *
 * void StateGreen::deinit(DataType & data) const
 * {
 *     std::cout << "StateGreen::deinit" << std::endl;
 * }
 *
 *
 * void StateYellow::init(DataType & data) const
 * {
 *     std::cout << "StateYellow::init" << std::endl;
 *     data.remainingDurationInPhase = 3;
 *     data.trafficLight.red = false;
 *     data.trafficLight.yellow = true;
 *     data.trafficLight.green = false;
 * }
 *
 * AbstractState<DataType> const & StateYellow::process(DataType & data) const
 * {
 *     std::cout << "StateYellow::process" << std::endl;
 *
 *     --data.remainingDurationInPhase;
 *     if (0 == data.remainingDurationInPhase)
 *     {
 *         return stateRed;
 *     }
 *     else
 *     {
 *         return *this;
 *     }
 * }
 *
 * void StateYellow::deinit(DataType & data) const
 * {
 *     std::cout << "StateYellow::deinit" << std::endl;
 * }
 *
 * // example usage
 *
 * int main()
 * {
 *     Statemachine statemachine(stateRed);
 *
 *     DataType data;
 *
 *     for (size_t index = 0; index < 40; ++index)
 *     {
 *         statemachine.process(data);
 *         std::cout << data.trafficLight << "    (" << index << ")" << std::endl;
 *     }
 *
 *     return 0;
 * }
 *
 **/

} // namespace Helpers
