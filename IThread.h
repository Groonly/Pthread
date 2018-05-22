class IThread{
public:
  IThread(){}
  virtual ~IThread(){}
  virtual  Run() = 0;
};
