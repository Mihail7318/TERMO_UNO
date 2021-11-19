void shedule_window(int AM_H, int AM_M, int PM_H, int PM_M, int TEMP_AM, int TEMP_PM)
{
  lcd.setCursor(0,0);
  if(AM_M == 0){
    lcd.print("AM: "+(String)AM_H+":"+"00"+" T:"+(String)TEMP_AM);
    }else{
    lcd.print("AM: "+(String)AM_H+":"+(String)AM_M+" T:"+(String)TEMP_AM);
    }
  lcd.setCursor(0,1);
    if(PM_M == 0){
    lcd.print("PM: "+(String)PM_H+":"+"00"+" T:"+(String)TEMP_PM);
    }else{
    lcd.print("PM: "+(String)PM_H+":"+(String)PM_M+" T:"+(String)TEMP_PM);
    }
  }

void main_window(int Tin, int Tout)
{
  lcd.setCursor(0,0);
  lcd.write(2);
  lcd.setCursor(1,0); 
  lcd.print(h);
  lcd.print(":");
  lcd.print(m);
  lcd.setCursor(9,0);
  lcd.write(0);
  lcd.setCursor(13,0);
  lcd.write(1);
  lcd.setCursor(0,1);
  lcd.print("Tin:"+(String)Tin+" "+"Tout:" + (String)Tout);
}

void set_temp_w(int ust, bool fl){
  if(!fl){lcd.clear();}
  lcd.setCursor(0,0);
  lcd.print("SET_TEMPERATURE");
  lcd.setCursor(6,1);
  lcd.print(ust);
  }
