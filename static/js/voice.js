// the js file for the voice input feature 
// https://web.dev/articles/media-recording-audio
// i REALLY tried to get everything work using C++ but it doesn't seem to be worth the effort
// it just feels like i am trying to fit a square peg into a round hole
// so i am pretty much stuck with js for now


const voiceInput = () => {
  // check if the browser supports the MediaDevices API
  if (!navigator.mediaDevices || !navigator.mediaDevices.getUserMedia) {
    console.error("MediaDevices API or getUserMedia method not supported.");
    return Promise.reject(new Error('Get a proper browser dude.'));;
  }


  let mediaRecorder;
  let audioChunks = [];
  let isRecording = false;
  
  // const player = document.getElementById('player');
  const recordButton = document.getElementById('recordButton');
  
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
              // player.src = audioUrl;
          };
      })
      .catch(error => console.error('Error accessing the microphone:', error));
  
  


  return (
    
  )
}

export default voiceInput;