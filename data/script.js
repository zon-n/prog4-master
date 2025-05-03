window.addEventListener("load", getReadings);

var chartT = new Highcharts.Chart({
  chart: {
    type: "bar",
    renderTo: "chart-telemetry",
  },

  series: [
    {
      name: "Vitesse input",
      type: "bar",
      data: [
        { y: 0, color: "#101D42" },
        { y: 0, color: "#00A6A6" },
        { y: 0, color: "#8B2635" },
        { y: 0, color: "#71B48D" },
      ],
      color: "303033",
    },
    {
      name: "Vitesse output",
      type: "bar",
      data: [
        { y: 0, color: "#1F4B79" },
        { y: 0, color: "#00D0D0" },
        { y: 0, color: "#B64B62" },
        { y: 0, color: "#A8C6A2" },
      ],
      color: "#A9A9A9",
    },
  ],
  title: {
    text: undefined,
  },
  xAxis: {
    categories: ["Avant Gauche", "Avant Droit", "Arrière Gauche", "Arrière Droit"],
  },
  yAxis: {
    title: {
      text: "Throttle Percentage (%)",
    },
    min: 0, // Set the minimum value
    max: 100, // Set the maximum value
  },

  credits: {
    enabled: false,
  },
});

function plotJSON(jsonValue) {
  var wheelData = {
    input: {
      front_left: Number(jsonValue["front_left_input"]),
      front_right: Number(jsonValue["front_right_input"]),
      rear_left: Number(jsonValue["rear_left_input"]),
      rear_right: Number(jsonValue["rear_right_input"]),
    },
  
    output: {
      front_left: Number(jsonValue["front_left_output"]),
      front_right: Number(jsonValue["front_right_output"]),
      rear_left: Number(jsonValue["rear_left_output"]),
      rear_right: Number(jsonValue["rear_right_output"]),
    },
  };

  chartT.series[0].setData([
    wheelData.input.front_left,
    wheelData.input.front_right,
    wheelData.input.rear_left,
    wheelData.input.rear_right,
  ]);

  chartT.series[1].setData([
    wheelData.output.front_left,
    wheelData.output.front_right,
    wheelData.output.rear_left,
    wheelData.output.rear_right,
  ]);
}

function getReadings() {
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      var myObj = JSON.parse(this.responseText);
      console.log(myObj);
      plotJSON(myObj);
    }
  };
  xhr.open("GET", "/readings", true);
  xhr.send();
}

if (!!window.EventSource) {
  var source = new EventSource("/events");

  source.addEventListener(
    "open",
    function (e) {
      console.log("Events Connected");
    },
    false
  );

  source.addEventListener(
    "error",
    function (e) {
      if (e.target.readyState != EventSource.OPEN) {
        console.log("Events Disconnected");
      }
    },
    false
  );

  source.addEventListener(
    "message",
    function (e) {
      console.log("message", e.data);
    },
    false
  );

  source.addEventListener(
    "new_readings",
    function (e) {
      console.log("new_readings", e.data);
      var myObj = JSON.parse(e.data);
      console.log(myObj);
      plotJSON(myObj);
    },
    false
  );
}

const jsonData = {
  input_orientation: "45°",
  output_orientation: "43°",
  value3: "50%",
  value4: "85%",
  value5: "OK",
};

function updateTextBoxes(data) {
  document.getElementById("input-orientation").value = data.input_orientation;
  document.getElementById("output-orientation").value = data.output_orientation;
  document.getElementById("value3").value = data.value3;
  document.getElementById("value4").value = data.value4;
  document.getElementById("value5").value = data.value5;
}

document.addEventListener("DOMContentLoaded", () => {
  updateTextBoxes(jsonData);
});
