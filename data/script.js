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

var chart2 = new Highcharts.Chart({
  chart: {
    type: "spline",
    renderTo: "chart-analysis",
  },

  series: [
    {
      name: "Angle input",
      type: "spline",
      color: "#8B2635",
    },
    {
      name: "Angle output",
      type: "spline",
      color: "#71B48D",
    },
  ],

  title: {
    text: undefined,
  },

  xAxis: {
    title: {
      text: "Temps",
    },
  },

  yAxis: {
    title: {
      text: "Angle en degrés",
    },
  },
  plotOptions: {
    spline: {
      dataLabels: {
        enabled: false,
      },
      marker: {
        enabled: false,
      },
    },
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
    Math.abs(wheelData.input.front_left),
    Math.abs(wheelData.input.front_right),
    Math.abs(wheelData.input.rear_left),
    Math.abs(wheelData.input.rear_right),
  ]);

  chart1.series[1].setData([
    Math.abs(wheelData.output.front_left),
    Math.abs(wheelData.output.front_right),
    Math.abs(wheelData.output.rear_left),
    Math.abs(wheelData.output.rear_right),
  ]);

  // Ajouter les points d'entrée et de sortie au graphique d'analyse

  if (chart2.series[0].data.length > 75) {
    chart2.series[0].data[0].remove();
  }

  if (chart2.series[1].data.length > 75) {
    chart2.series[1].data[0].remove();
  }

  chart2.series[0].addPoint(Number(jsonValue["input_y"]));
  chart2.series[1].addPoint(Number(jsonValue["output_y"]));
  console.log(chart2.series[0].data);
  console.log(chart2.series[1].data);
}

function updateTextBoxes(data) {
  xInput = Number(data.input_x);
  yInput = Number(data.input_y);
  xOutput = Number(data.output_x);
  yOutput = Number(data.output_y);

  document.getElementById("input-orientation").value = yInput.toFixed(2);
  document.getElementById("output-orientation").value = yOutput.toFixed(2);
  document.getElementById("input-speed").value = xInput.toFixed(2);
  document.getElementById("output-speed").value = xOutput.toFixed(2);
  document.getElementById("kp").value = data.kp.toFixed(2);
  document.getElementById("ki").value = data.ki.toFixed(2);
  document.getElementById("kd").value = data.kd.toFixed(2);

  console.log("Input Orientation: ", yInput, "Output Orientation: ", yOutput);
  console.log("Input Speed: ", xInput, "Output Speed: ", xOutput);
}

function vectorToMecanum(x, y) {
  speed = x;
  angle = y;

  vX = Math.round(speed * Math.sin((angle * Math.PI) / 180) * 100) / 100;
  vY = Math.round(speed * Math.cos((angle * Math.PI) / 180) * 100) / 100;
  console.log("Vitesse X: ", vX, "Vitesse Y: ", vY);

  mecanumSpeed = [
    Math.round((vY - vX) * 100) / 100, // Avant Gauche
    Math.round((vY + vX) * 100) / 100, // Avant Droit
    Math.round((vY + vX) * 100) / 100, // Arrière Gauche
    Math.round((vY - vX) * 100) / 100, // Arrière Droit
  ];
  console.log("Mecanum Speed: ", mecanumSpeed);
  console.log("Vitesse input: ", speed, "Angle: ", angle);

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
