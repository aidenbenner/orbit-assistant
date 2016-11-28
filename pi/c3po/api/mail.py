import imaplib
import re
from email import policy
from email.parser import Parser
from c3po.config.email import username

# TODO change password and supply from ENV
password = 'ThankMrGoose'


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
        - number of emails to get
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

        result.append({'sender': sender, 'subject': subject, 'body': body})

    m.close()
    m.logout()
    return result


def send_mail(options):
    """
    options:
        - recipient
        - body
    """
    recipient, body = options

    m = imaplib.IMAP4_SSL('imap.gmail.com')
    m.login(username, password)

    m.sendmail(username, recipient, body)
