// Initialisation de l'affichage
window.addEventListener("load", getReadings);

// Configuration des diagrammes
var chart1 = new Highcharts.Chart({
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
    categories: [
      "Avant Gauche",
      "Avant Droit",
      "Arrière Gauche",
      "Arrière Droit",
    ],
  },
  yAxis: {
    title: {
      text: "Throttle Percentage (%)",
    },
    min: 0,
    max: 150,
  },
  credits: {
    enabled: false,
  },
});

// Fontions de logique Javascript
function plotJSON(jsonValue) {
  mecanumSpeedInput = vectorToMecanum(
    Number(jsonValue["input_x"]),
    Number(jsonValue["input_y"])
  );

  mecanumSpeedOutput = vectorToMecanum(
    Number(jsonValue["output_x"]),
    Number(jsonValue["output_y"])
  );

  var wheelData = {
    input: {
      front_left: mecanumSpeedInput[0],
      front_right: mecanumSpeedInput[1],
      rear_left: mecanumSpeedInput[2],
      rear_right: mecanumSpeedInput[3],
    },
    output: {
      front_left: mecanumSpeedOutput[0],
      front_right: mecanumSpeedOutput[1],
      rear_left: mecanumSpeedOutput[2],
      rear_right: mecanumSpeedOutput[3],
    },
  };

  chart1.series[0].setData([
    wheelData.input.front_left,
    wheelData.input.front_right,
    wheelData.input.rear_left,
    wheelData.input.rear_right,
  ]);

  chart1.series[1].setData([
    wheelData.output.front_left,
    wheelData.output.front_right,
    wheelData.output.rear_left,
    wheelData.output.rear_right,
  ]);
}

function updateTextBoxes(data) {
  xInput = Number(data.input_x);
  yInput = Number(data.input_y);
  xOutput = Number(data.output_x);
  yOutput = Number(data.output_y);

  document.getElementById("input-orientation").value = (
    Math.atan2(yInput, xInput) *
    (180 / Math.PI)
  ).toFixed(2);
  document.getElementById("output-orientation").value = (
    Math.atan2(yOutput, xOutput) *
    (180 / Math.PI)
  ).toFixed(2);
  document.getElementById("value3").value = data.value3;
  document.getElementById("value4").value = data.value4;
  document.getElementById("value5").value = data.value5;
}

function vectorToMecanum(x, y) {
  speed = Math.sqrt(x * x + y * y);
  angle = Math.atan2(y, x);
  mecanumSpeed = [
    speed * Math.cos(angle + Math.PI / 4),
    speed * Math.sin(angle + Math.PI / 4),
    speed * Math.cos(angle - Math.PI / 4),
    speed * Math.sin(angle - Math.PI / 4),
  ];
  return mecanumSpeed;
}

// Fonctions de récupération des données
function getReadings() {
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      var myObj = JSON.parse(this.responseText);
      console.log(myObj);
      plotJSON(myObj);
      updateTextBoxes(myObj);
    }
  };
  xhr.open("GET", "/readings", true);
  xhr.send();
}

// Fonction de mise à jour des données
if (!!window.EventSource) {
  var source = new EventSource("/events");

  source.addEventListener("open", function (e) {
    console.log("Events Connected");
  });

  source.addEventListener("error", function (e) {
    if (e.target.readyState != EventSource.OPEN) {
      console.log("Events Disconnected");
    }
  });

  source.addEventListener("message", function (e) {
    console.log("message", e.data);
  });

  source.addEventListener("new_readings", function (e) {
    console.log("new_readings", e.data);
    var myObj = JSON.parse(e.data);
    plotJSON(myObj);
    updateTextBoxes(myObj);
  });
}
