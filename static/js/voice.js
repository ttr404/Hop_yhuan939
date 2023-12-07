// the js file for the whole voice input feature 
// https://web.dev/articles/media-recording-audio
// i REALLY tried to get everything work using C++ but it doesn't seem to be worth the effort
// it just feels like i am trying to fit a square peg into a round hole
// and use that square peg to hammer a nail into a wall
// and use that nail to hang a picture frame
// and use that picture frame to cover a hole in the wall
// and use that hole in the wall to hide a safe
// and use that safe to store a key
// and use that key to open a door
// and use that door to enter a room
// and use that room to store a box
// and use that box to store a square peg
// and use that square peg to hammer a nail into a wall
// so i am pretty much stuck with js for now
// javascript is fucking retarded

// handle port number 
const SERVER_URL = window.location.protocol + "//" + window.location.hostname + ":" + window.location.port;
const TOKEN = "r8_X1ErZq5FO5J4XZc00HphUvYTWLr3zdP1utzio"

// check if the browser supports the MediaDevices API
if (!navigator.mediaDevices || !navigator.mediaDevices.getUserMedia) {
  console.error("MediaDevices API or getUserMedia method not supported.");
  Promise.reject(new Error('Get a proper browser dude.'));;
}

let mediaRecorder;
let audioChunks = [];
let isRecording = false;

// a helper function for saving the blob object
function blobToBase64(blob) {
  const reader = new FileReader();
  reader.readAsDataURL(blob);
  return new Promise(resolve => {
    reader.onloadend = () => {
      resolve(reader.result);
    };
  });
};

// fuck this bullshit
// FUCK
async function refetch(id) {
  // if we have tried more than 5 times then fuck it 
  let maxRetries = 5;
  let attempt = 0;
  // the interval between each attempt
  let interval = 1500;

  // we probably have to try to refetch the result multiple times
  while (attempt < maxRetries) {
    try {
      const response = await fetch(SERVER_URL + '/refetch/' + id);
      console.log('Attempt:', attempt, 'HTTP Response:', response);

      if (!response.ok) {
        throw new Error('Network response was not ok');
      }

      const responseText = await response.text();
      const jsonResponse = JSON.parse(JSON.parse(responseText));
      // console.log('JSON Response:', jsonResponse);

      // if it is succeeded
      if (jsonResponse['status'] === 'succeeded') {
        console.log('Success:', jsonResponse);
        // console.log('Final result:', jsonResponse['output']['transcription']);
        // punctuations to be removed
        var punctuation = /[\.,?!]/g;
        var strpRet = jsonResponse['output']['transcription'].trim().replace(punctuation, "");

        // return the transcription of what the user said
        return strpRet;
      } else {
        console.log('Status not succeeded, retrying...');
      }
    } catch (error) {
      console.error('Fetch error:', error);
    }
    // wait for a while before trying again because this shit is retarded and i cannot find a better way of doing it
    await new Promise(resolve => setTimeout(resolve, interval));
    attempt++;
  }

  return "Fetching Failed. Max retries reached."
}

// get the audio from the microphone
async function getAudio() {
  console.log('Requesting microphone access...');
  try {
    const stream = await navigator.mediaDevices.getUserMedia({ audio: true });
    mediaRecorder = new MediaRecorder(stream);
    mediaRecorder.ondataavailable = event => {
      audioChunks.push(event.data);
    };
    mediaRecorder.onstop = async () => {
      try {
        // convert the audio to a blob
        const audioBlob = new Blob(audioChunks, { type: 'audio/mp3' });

        // URLs are generated for debugging purposes only
        const audioUrl = URL.createObjectURL(audioBlob);
        const recordedAudio = document.createElement("audio");
        recordedAudio.src = audioUrl;
        console.log('recordedAudio:', recordedAudio);

        // send the audio to the server
        const formData = new FormData();
        const file = await blobToBase64(audioBlob);
        formData.append('file', file);
        // console.log('formData:', formData);

        // try to send the audio to the server
        try {
          const response = await fetch(SERVER_URL + "/voiceUpload", {
            method: 'POST',
            body: formData
          });
          // the data should be 
          const data = await response.json();
          const id = data.Rep_id;
          console.log('id:', id);

          // refetch the result
          refetch(id).then(result => {
            // we are getting the final output hoorayyyyyyy!
            console.log('Final result:', result);
            document.getElementById('voiceResultInput').value = result;
          }).catch(error => {
            console.error('Refetch error:', error);
          });

        } catch (error) {
          console.error('Error:', error);
        }

      } catch (error) {
        console.error('Error in onstop:', error);
      }
    };

  } catch (error) {
    console.error('Error accessing the microphone:', error);
    throw error;
  }
}

// const player = document.getElementById('player');
const recordButton = document.getElementById('recordButton');

// event listener for the start and stop buttons
recordButton.addEventListener('click', toggleRecording);

// toggle recording
async function toggleRecording() {
  console.log('toggleRecording()');
  if (isRecording) {
    console.log('stopping recording');
    // stop recording
    try {
      await mediaRecorder.stop();
      isRecording = false;
      // swap between the icon
      recordButton.classList.toggle('recording');
      recordButton.innerHTML = '<i class="ri-mic-fill"></i>';
    } catch (error) {
      console.error('Error stopping recording:', error);
    }

  } else {
    console.log('start recording');
    // start recording
    audioChunks = [];
    try {
      await getAudio();
      console.log('mediaRecorder:', mediaRecorder);
      mediaRecorder.start();
      recordButton.classList.toggle('recording');
      recordButton.innerHTML = '<i class="ri-stop-circle-fill"></i>';
      isRecording = true;
    } catch (error) {
      console.error('Error starting recording:', error);
    }
  }
}