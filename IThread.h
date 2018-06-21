class IThread{
public:
  IThread(){}
  virtual ~IThread(){}
  int virtual Run() = 0;
};
