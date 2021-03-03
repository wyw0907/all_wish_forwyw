import sys
import smtplib
from email.mime.text import MIMEText

mail_host="mail.myhexin.com"
mail_user="wuyunwei@myhexin.com"
mail_pwd="******"

def send_mail(mailto, subject, content) :
    msg = MIMEText(content.encode("gbk"), _subtype = "plain", _charset= "gbk")
    msg["From"] = mail_user
    msg["Subject"] = subject
    msg["To"] = mailto

    try:
        s = smtplib.SMTP_SSL(mail_host, 465)
        s.login(mail_user, mail_pwd)
        s.sendmail(mail_user, mailto, msg.as_string())
        s.close()
    except Exception as e:
        print("Exception: %s"% e)

if __name__=="__main__":
    send_mail("wuyunwei@myhexin.com", "测试主题", "测试邮件发送，内容")