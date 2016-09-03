int uniqueDereferenceID = 0;
int globalObjectCount = 0;
class Collectable {
public:
  int references;
  int passingID;
  Collectable() : references(0), passingID(0) {
    globalObjectCount++;
    //cout<<"NEW Count: "<<globalObjectCount<<" | "<<this<<endl;
  }
  virtual ~Collectable() {}
  void reference() {
    if(this == nullptr)
      return;
    //cout<<"@@@> Referencing "<<this<<" to "<<(references+1)<<endl;
    references++;
  }
  bool finalize() {
    if(this == nullptr)
      return false;
    //if(disabled)return false;
    //cout<<"@@@> Finalizing "<<this<<" WITH "<<references<<endl;
    if(references == 0) {
      ////cout<<"||||||||-START||||||||||Deleting "<<this<<endl;
      uniqueDereferenceID++;
      recursiveDereferenceStep();
      ////cout<<"||||||||-END||||||||||Deleting "<<this<<endl;
      globalObjectCount--;
      //cout<<"DEL Count: "<<globalObjectCount<<" | "<<this<<endl;
      delete this;
      return true;
    } else return false;
  }
  void dereferenceProtect() { 
    if(this == nullptr)
      return;
    if(references > 0)
      references--;
  }
  void dereference() {
    uniqueDereferenceID++;
    dereferenceStep();
  }
  void dereferenceStep() {
    if(this == nullptr)
      return;
    ////cout<<"@@@> Dereferencing "<<this<<endl;
    if(uniqueDereferenceID == passingID) {
      ////cout<<"ALREADY PASSED"<<endl;
      return;
    }
    if(!finalize()) {
      references--;
      passingID = uniqueDereferenceID;
      finalize();
    } 
  }
  virtual void recursiveDereferenceStep() = 0;
};
