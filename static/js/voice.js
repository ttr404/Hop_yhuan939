// the js file for the voice input feature 
// https://web.dev/articles/media-recording-audio
let mediaRecorder;
let audioChunks = [];
let isRecording = false;

const player = document.getElementById('player');
const recordButton = document.querySelector('.form button:nth-child(3)');

// event listener for the start and stop buttons
recordButton.addEventListener('click', toggleRecording);

// toggle recording
function toggleRecording() {
  // change the icon
  const icon = recordButton.querySelector('i'); 
  if (isRecording) {
      // stop recording
      mediaRecorder.stop();
      recordButton.innerHTML = '<i class="ri-send-plane-fill"></i>';
    } else {
      // start recording
      audioChunks = [];
      mediaRecorder.start();
      recordButton.innerHTML = '<i class="ri-stop-circle-fill"></i>';
  }
  isRecording = !isRecording;
}

// request microphone access
navigator.mediaDevices.getUserMedia({ audio: true })
    .then(stream => {
        mediaRecorder = new MediaRecorder(stream);

        mediaRecorder.ondataavailable = event => {
            audioChunks.push(event.data);
        };

        mediaRecorder.onstop = () => {
            const audioBlob = new Blob(audioChunks, { type: 'audio/mp3' });
            const audioUrl = URL.createObjectURL(audioBlob);
            player.src = audioUrl;
        };
    })
    .catch(error => console.error('Error accessing the microphone:', error));



    
// const voiceInput = () => {
//   // check if the browser supports the MediaDevices API
//   if (!navigator.mediaDevices || !navigator.mediaDevices.getUserMedia) {
//     console.error("MediaDevices API or getUserMedia method not supported.");
//     return;
//   }



//   return (
    
//   )
// }
