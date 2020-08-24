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

} cntrlMode;

void ControlMode::setControlType(control_type typex) {
  type = typex;
}

control_type ControlMode::getControlType() {
  return type;
}

/*
  2 cntrls fixd temp and by graph
  always check koef set by user
*/

void ControlMode::cntrlByMaxTemp() {
  /*
    not higher then max set te temperature.
    float koef, look at reference in fb chat
    open valve and wait, 2 degrees lower then max.
  */
}

void ControlMode::cntrlByGraph() {
/*
  graph is const array
  do array for temp and compare
*/
}

void ControlMode::cntrlByLh() {

}

void ControlMode::cntrlByNurk() {

}

void ControlMode::cntrlByKoef() {

}

void ControlMode::cntrlByHour() {

}
