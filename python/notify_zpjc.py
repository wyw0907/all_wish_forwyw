import lib.mail as mail
import datetime


if __name__=="__main__":
    mail_list = [ 
        { 
            "date": "星期天",
            "users": ["wuyunwei@myhexin.com"], 
        },
        { 
            "date": "星期一",
            "users": ["wuyunwei@myhexin.com"], 
        },
        { 
            "date": "星期二",
            "users": ["wuyunwei@myhexin.com"], 
        },
        { 
            "date": "星期三",
            "users": ["wuyunwei@myhexin.com"], 
        },
        { 
            "date": "星期四",
            "users": ["wuyunwei@myhexin.com"], 
        }
    ]
    weekday = datetime.date.today().isoweekday()
    print(weekday)
    weekday = (weekday + 1) % 7
    if weekday <= len(mail_list) and weekday != 0:
        print(mail_list[weekday - 1])
        info = dict(mail_list[weekday - 1])
        content = u"今天%s, 明天记得早盘检查" % info["date"]
        mail.send_mail(users = list(info["users"]), "每日提醒", content)
