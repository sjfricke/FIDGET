// Twilio Credentials
const accountSid = 'ACef58797557d275b055feb5fd2d66e416';
const authToken = '97062eda4e85e5b10ddd8541d1ff8af3';

// require the Twilio module and create a REST client
const client = require('twilio')(accountSid, authToken);

client.messages
  .create({
    to: '+2068897536',
    from: '+13236895513',
    body: 'Hello from Charlie: The original Soylent Fidget Spinner App!',
  })
  .then(message => console.log(message.sid));
