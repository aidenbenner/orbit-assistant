import imaplib
import re
import smtplib
from email import policy
from email.parser import Parser

# TODO change password and supply from ENV
password = 'ThankMrGoose'
username = 'se101bot@gmail.com'


def trim_body(body):
    n = re.sub(r'Content-Type: text/plain; charset=UTF-8',
               '',
               body,
               flags=re.IGNORECASE)
    n = n.replace('\n', '')
    return n


def get_mails(options):
    """
    options:
        - max number of emails to get
    """
    number_emails = options[0]

    m = imaplib.IMAP4_SSL('imap.gmail.com')
    m.login(username, password)
    m.select('"[Gmail]/Inbox"')

    resp, items = m.search(None, 'ALL')

    mail_ids = items[0].split()

    if mail_ids > number_emails:
        mail_ids = number_emails

    result = []

    for mail_id in mail_ids:
        resp, data = m.fetch(mail_id, '(RFC822)')
        email_body = data[0][1].decode('utf8')
        mail = Parser(policy=policy.default).parsestr(email_body)

        sender = mail['From']
        subject = mail['Subject']
        body = mail.get_body(preferencelist=('plain'))
        body = trim_body(body.as_string())

        result.append({'sender': sender, 'subject': subject, 'body': body,
                       'to': username})

    m.close()
    m.logout()
    return result


def send_mail(options):
    """
    options:
        - recipient
        - subject
        - body
    """
    recipient, subject, body = options

    message = 'Subject: %s\n\n%s' % (subject, body)

    server = smtplib.SMTP('smtp.gmail.com:587')
    server.ehlo()
    server.starttls()
    server.login(username, password)
    server.sendmail(username, recipient, message)
    server.quit()

    return {'status': "SUCCESS"}
