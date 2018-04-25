#ifndef _USER_MANNUAL_H
#define _USER_MANNUAL_H

//admin
void admin_manual(char* in);
void healthcare_manual(char* in);

void admin_create_accnt_manual(char* new_usr_grp,char* new_usr_name,char* new_pswd);

void admin_del_accnt_manual(char* del_usr_name);

//healthcare
void healthcare_manual(char* in);

void healthcare_create_accnt_manual(char* new_patient_name,char* new_patient_insurance,char* new_record);

void healthcare_modify_manual(char* item_name,char* new_item);

void healthcare_qry_manual(char* qry_name);

//common
void modify_manual(char* item_name,char* new_item);

void qry_manual(char* qry_usr_name);

void chg_pswd_manual(char* chg_usr_pswd);


void insurance_manual(char* in);
#endif
