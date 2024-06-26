namespace Helpers
{

template<typename DATA_TYPE>
class AbstractState
{
public:

    virtual void init(DATA_TYPE & data) = 0;

    virtual AbstractState *process(DATA_TYPE & data) = 0;

    virtual void deinit(DATA_TYPE & data) = 0;

protected:

    AbstractState() = default;

    AbstractState(const AbstractState &) = delete;
    AbstractState(AbstractState &&) = delete;
    AbstractState &operator=(const AbstractState &) = delete;
    AbstractState &operator=(AbstractState &&) = delete;
};


template <typename DATA_TYPE>
class Statemachine
{
public:

    Statemachine(AbstractState<DATA_TYPE> * const startState)
        : previousState_(nullptr)
        , currentState_(startState)
    {
        // intentionally empty
    }

    void process(DATA_TYPE & data)
    {
        if (previousState_ != currentState_)
        {
            currentState_->init(data);
            previousState_ = currentState_;
        }

        AbstractState<DATA_TYPE> * const nextState = currentState_->process(data);

        if (nextState != currentState_)
        {
            currentState_->deinit(data);
            currentState_ = nextState;
        }
    }

private:

    AbstractState<DATA_TYPE> * previousState_;
    AbstractState<DATA_TYPE> * currentState_;

};


/* ----- Example -----
 *
 * #include <iostream>
 *
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
 * class StateRed : public AbstractState<DataType>
 * {
 *     void init(DataType & data) override;
 *
 *     AbstractState * process(DataType & data) override;
 *
 *     void deinit(DataType & data) override;
 * };
 *
 * static StateRed stateRed;
 *
 *
 * class StateRedYellow : public AbstractState<DataType>
 * {
 *     void init(DataType & data) override;
 *
 *     AbstractState * process(DataType & data) override;
 *
 *     void deinit(DataType & data) override;
 * };
 *
 * static StateRedYellow stateRedYellow;
 *
 *
 * class StateGreen : public AbstractState<DataType>
 * {
 *     void init(DataType & data) override;
 *
 *     AbstractState * process(DataType & data) override;
 *
 *     void deinit(DataType & data) override;
 * };
 *
 * static StateGreen stateGreen;
 *
 *
 * class StateYellow : public AbstractState<DataType>
 * {
 *     void init(DataType & data) override;
 *
 *     AbstractState * process(DataType & data) override;
 *
 *     void deinit(DataType & data) override;
 * };
 *
 * static StateYellow stateYellow;
 *
 *
 *
 *
 * void StateRed::init(DataType & data)
 * {
 *     std::cout << "StateRed::init" << std::endl;
 *     data.remainingDurationInPhase = 10;
 *     data.trafficLight.red = true;
 *     data.trafficLight.green = false;
 *     data.trafficLight.yellow = false;
 * }
 *
 * AbstractState<DataType> * StateRed::process(DataType & data)
 * {
 *     std::cout << "StateRed::process" << std::endl;
 *
 *     --data.remainingDurationInPhase;
 *     if (0 == data.remainingDurationInPhase)
 *     {
 *         return &stateRedYellow;
 *     }
 *     else
 *     {
 *         return this;
 *     }
 * }
 *
 * void StateRed::deinit(DataType & data)
 * {
 *     std::cout << "StateRed::deinit" << std::endl;
 * }
 *
 *
 * void StateRedYellow::init(DataType & data)
 * {
 *     std::cout << "StateRedYellow::init" << std::endl;
 *     data.remainingDurationInPhase = 2;
 *     data.trafficLight.red = true;
 *     data.trafficLight.yellow = true;
 *     data.trafficLight.green = false;
 * }
 *
 * AbstractState<DataType> * StateRedYellow::process(DataType & data)
 * {
 *     std::cout << "StateRedYellow::process" << std::endl;
 *
 *     --data.remainingDurationInPhase;
 *     if (0 == data.remainingDurationInPhase)
 *     {
 *         return &stateGreen;
 *     }
 *     else
 *     {
 *         return this;
 *     }
 * }
 *
 * void StateRedYellow::deinit(DataType & data)
 * {
 *     std::cout << "StateRedYellow::deinit" << std::endl;
 * }
 *
 *
 * void StateGreen::init(DataType & data)
 * {
 *     std::cout << "StateGreen::init" << std::endl;
 *     data.remainingDurationInPhase = 5;
 *     data.trafficLight.red = false;
 *     data.trafficLight.yellow = false;
 *     data.trafficLight.green = true;
 * }
 *
 * AbstractState<DataType> * StateGreen::process(DataType & data)
 * {
 *     std::cout << "StateGreen::process" << std::endl;
 *
 *     --data.remainingDurationInPhase;
 *     if (0 == data.remainingDurationInPhase)
 *     {
 *         return &stateYellow;
 *     }
 *     else
 *     {
 *         return this;
 *     }
 * }
 *
 * void StateGreen::deinit(DataType & data)
 * {
 *     std::cout << "StateGreen::deinit" << std::endl;
 * }
 *
 *
 * void StateYellow::init(DataType & data)
 * {
 *     std::cout << "StateYellow::init" << std::endl;
 *     data.remainingDurationInPhase = 3;
 *     data.trafficLight.red = false;
 *     data.trafficLight.yellow = true;
 *     data.trafficLight.green = false;
 * }
 *
 * AbstractState<DataType> * StateYellow::process(DataType & data)
 * {
 *     std::cout << "StateYellow::process" << std::endl;
 *
 *     --data.remainingDurationInPhase;
 *     if (0 == data.remainingDurationInPhase)
 *     {
 *         return &stateRed;
 *     }
 *     else
 *     {
 *         return this;
 *     }
 * }
 *
 * void StateYellow::deinit(DataType & data)
 * {
 *     std::cout << "StateYellow::deinit" << std::endl;
 * }
 *
 * // example usage
 *
 * int main()
 * {
 *     Statemachine statemachine(&stateRed);
 *
 *     DataType data;
 *
 *     for (size_t index = 0; index < 40; ++index)
 *     {
 *         statemachine.process(data);
 *
 *         // Typically the output should happen inside the process command,
 *         // as otherwise the deinit()-call might show strange results at this point.
 *         std::cout << data.trafficLight << "    (" << index << ")" << std::endl;
 *     }
 *
 *     return 0;
 * }
 *
 **/

} // namespace Helpers
