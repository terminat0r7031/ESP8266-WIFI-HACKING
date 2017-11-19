(function(monitor) {
    //Private Property
    var intervalGetData;
    var numCheck = 0;
    var time = 10, channel = 1;
    var timeDelay = 2000;
    var maxGoiTin = 400;
    var value = {bc: 0, de: 0, dis: 0};

    var cv = _function.getEleByID("canvas"),
    		ctx = cv.getContext("2d");
    var wi = window.innerWidth*80/100;
    var w = cv.width = wi > 500?500:wi;
    var h = cv.height = cv.width*2/3;

    var dramX = w/(time/2),
    		dramY = h/(maxGoiTin/5);

    var x1,y1,x2,y2,x3,y3;
    //Public Method
    monitor.initChart = ()=>{
    	cv.style.backgroundColor = "black";
    	ctx.font = "10px Arial";
			ctx.fillStyle = "white";

    	dramX = w/(time/2),
			dramY = h/(maxGoiTin/10);

    	//draw cot doc
    	ctx.beginPath();
    	ctx.strokeStyle = "white";
    	ctx.moveTo(dramX/4, dramY);
			ctx.lineTo(dramX/4, h-dramY);
			ctx.stroke();

			ctx.moveTo(dramX/4,dramY);
			ctx.lineTo(dramX/4 - 5, dramY + 5);
			ctx.stroke();

			ctx.moveTo(dramX/4,dramY);
			ctx.lineTo(dramX/4 + 5, dramY + 5);
			ctx.stroke();
				//draw num cot
				for(let i = 0; i <= maxGoiTin/50; i++){
					ctx.fillText( (maxGoiTin - i*50).toString() ,dramX/13, h - dramY - (h - 2*dramY)*(8-i)/8);
				}
				ctx.fillText("(frames)", 30, 10);
			//draw doc ngang
			ctx.beginPath();
			ctx.strokeStyle = "white";
    	ctx.moveTo(dramX/4,h-dramY);
			ctx.lineTo(w - dramX/4, h-dramY);
			ctx.stroke();

			ctx.moveTo(w - dramX/4,h-dramY);
			ctx.lineTo(w - dramX/4 - 5, h-dramY - 5);
			ctx.stroke();

			ctx.moveTo(w - dramX/4,h-dramY);
			ctx.lineTo(w - dramX/4 - 5, h-dramY + 5);
			ctx.stroke();
				//draw num ngang
				for(let i = 1; i <= time/2; i++){
					ctx.fillText( (i*2).toString(), i*dramX, h);
				}
				ctx.fillText("(s)", w-dramX/4, h - dramY);

			x1=x2=x3 = dramX/4;
			y1=y2=y3 = h-dramY;
    }

    monitor.drawChart = ()=>{
    	ctx.beginPath();
    	ctx.strokeStyle = "blue";
    	ctx.moveTo(x1,y1);
				
		x1 = x1+(dramX*(time/2-1))/(time/2); y1 = (h - dramY - (h - 2*dramY)*value.bc/400);
		ctx.lineTo(x1, y1);
		ctx.stroke();

		ctx.beginPath();
		ctx.strokeStyle = "red";
		ctx.moveTo(x2,y2);
		x2 = x2+(dramX*(time/2-1))/(time/2); y2 = (h - dramY - (h - 2*dramY)*value.de/400);
		ctx.lineTo(x2, y2);
		ctx.stroke();

		ctx.beginPath();
		ctx.strokeStyle = "yellow";
		ctx.moveTo(x3,y3);
		x3 = x3+(dramX*(time/2-1))/(time/2); y3 = (h - dramY - (h - 2*dramY)*value.dis/400);
		ctx.lineTo(x3, y3);
		ctx.stroke();
    }

    monitor.getData = ()=>{
    	if(numCheck == 1){
	    	intervalGetData = setInterval(()=>{
					fetch("/sendMonitorResults.json").then(res=>res.json())
					.then(data=>{
						console.log(data);
						value.bc = parseInt(data.bc);
						value.de = parseInt(data.de);
						value.dis = parseInt(data.dis);
						monitor.drawChart();
					})
	    	}, timeDelay)
    	}
    }
    monitor.run = ()=>{
    	
    	_function.getEleByID("netMonitorBtn").classList.toggle("btn-inactive");
	  	_function.toggleDisplay("modal", 'show');
	  	_function.getEleByID("btnOkTiming").onclick = ()=>{
	  		_function.toggleDisplay("modal", 'show');
	  		time = _function.getEleByID("selectTiming").value;
	  		channel = _function.getEleByID("selectChannel").value;
	  		monitor.initChart();

		// setInterval(()=>{
		// 	value.bc = _function.randInt(20,50);
		// 	value.de = _function.randInt(20,150);
		// 	value.dis = _function.randInt(20,150);

		// 	console.log(value.bc);
		// 	monitor.drawChart();
		// }, 2000)

    		let url= `/startMonitorEnv.json?time=${time}&channel=${channel}`;
    		fetch(url).then(res=>res.json())
    		.then(data=>{
    			if(data){
    				let checkStatus = setInterval(()=>{
    					fetch('/checkStatus.json').then(res=>res.json())
    					.then(check=>{
    						if(check){
    							numCheck++;
    							monitor.getData();
    						} else {
    							clearInterval(intervalGetData);
    							clearInterval(checkStatus);
    							console.log('done');
    							return false;
    						}
    					})	
    				}, 500)
    				
    			}
    		})
    	}
    }
    //Private Method

}( window.monitor = window.monitor || {}));