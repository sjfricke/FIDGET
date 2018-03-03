// Twilio Credentials
const accountSid = 'AC879ee0070f72c20229bb64294455a8e0';
const authToken = 'ee3f263433a7528cc17731f42c33fc82';

// require the Twilio module and create a REST client
const client = require('twilio')(accountSid, authToken);

client.messages
  .create({
    to: '+2063213257',
    from: '+13236895583',
    body: 'This is the ship that made the Kessel Run in fourteen parsecs?',
  })
  .then(message => console.log(message.sid));
