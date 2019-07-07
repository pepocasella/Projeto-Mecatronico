#include "Statistic.h"

Statistic myStats;

float dado = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("Demo Statistics lib ");
  Serial.print(STATISTIC_LIB_VERSION);
  myStats.clear(); //explicitly start clean
}

void loop() {
  // put your main code here, to run repeatedly:
  int adin;

  for(int i=1; i<=50; i++)
  {
    float average=0;
    
    for(int j=1; j<=50; j++)
    {
      delay(100);
    
      adin=analogRead(A0);
      
      average=average+adin;
      
  }

  dado = average/50;
  
  myStats.add(dado);
  if (myStats.count() == 20)
  {
    Serial.print("  Count: ");
    Serial.println(myStats.count());
    Serial.print("  Min: ");
    Serial.println(myStats.minimum(),4);
    Serial.print("  Max: ");
    Serial.println(myStats.maximum(),4);
    Serial.print("  Average: ");
    Serial.println(myStats.average(), 4);
    // uncomment in Statistic.h file to use stdev
    #ifdef STAT_USE_STDEV
    Serial.print("    pop stdev: ");
    Serial.println(myStats.pop_stdev(), 4);
    Serial.print("  unbias stdev: ");
    Serial.println(myStats.unbiased_stdev(), 4);
    #endif
    Serial.println("=====================================");
    myStats.clear();
    delay(1000);

  }
  
    //Serial.println(adin);
    Serial.println(average/50);
    //delay(1000);
    
  }

}
