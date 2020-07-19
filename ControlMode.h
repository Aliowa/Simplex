typedef enum {
  MAXTEMP,
  GRAPH,
  LH,
  NURK,
  KOEF,
  HOUR
} control_type;

class ControlMode {
  public:
    //seter geter
    void setControlType(control_type);
    control_type getControlType();

  //Control mode methods

  void cntrlByMaxTemp(void);
  void cntrlByGraph(void);
  void cntrlByLh(void);
  void cntrlByNurk(void);
  void cntrlByKoef(void);
  void cntrlByHour(void);
    
  private:
    control_type type;
    
};

void ControlMode::setControlType(control_type typex) {
  type = typex;
}

control_type ControlMode::getControlType() {
  return type;
}

void ControlMode::cntrlByMaxTemp(){
  
}

void ControlMode::cntrlByGraph(){
  
}

void ControlMode::cntrlByLh(){
  
}

void ControlMode::cntrlByNurk(){
  
}

void ControlMode::cntrlByKoef(){
  
}

void ControlMode::cntrlByHour(){
  
}
