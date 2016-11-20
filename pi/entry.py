from c3po import SerialManager, config

sm = SerialManager('Serial', config.valid_events)
sm.start()
