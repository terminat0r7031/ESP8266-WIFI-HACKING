
(function( _function ) {
  //Private Method
  function getJson(url) {
		let rq = fetch(url);
		let dataJs;
		rq.then(res=>{
			res.json().then(data=>{
				dataJs = data;
			})
		})
		return dataJs;
	}
  //Public Method
	_function.getEleByID = (id) => {
		return document.getElementById(id);
	}
	_function.getJson = (url)=>{
		return getJson(url);
	}
	_function.toggleDisplay = (id, type)=>{
		document.getElementById(id).classList.toggle(type);
	}

	_function.hasClass = (id, className)=>{
		if(_function.getEleByID(id).className.indexOf(className) != -1) return true;
		return false;
	}

	_function.randInt = (min, max)=>{
		return Math.floor(Math.random()*(max - min + 1) + min);
	}
	
}( window._function = window._function || {}));

(function( _menu ) {
  //Private Property
  var isSlide = false;

  //Private Method
  function slideToggleMenu(e) {
		let id = e.target.id;

		if(id == 'menu-btn'){
			if(!isSlide)
				_function.getEleByID('main-menu').style.display = "block";
			else 
				_function.getEleByID('main-menu').style.display = "";
			isSlide = !isSlide;
		}
	}

	//Public Method
  _menu.init = ()=>{
  	_function.getEleByID('menu-btn').addEventListener("click", slideToggleMenu);
  }
  _menu.init();
}( window._menu = window._menu || {}));

(function(scan) {
  //Private Property
  var numSelectST = 0;

  //Public Method

  scan.Attack = (de_dis, isAll)=>{
  	Attack(de_dis, isAll);
  }

  scan.startScanAp = ()=>{
  	startScanAp();
  }
  scan.selectAP = (i)=>{
  	selectAP(i);
  }
  scan.startScanST = ()=>{
  	console.log("Start SCAN ST");
  	startScanST();
  }
  scan.selectST = (i)=>{
  	selectST(i);
  }
  //Private Method

  function Attack(de_dis, isAll) {

  	_function.getEleByID("attack-station-btn").classList.toggle("btn-inactive");
  	_function.getEleByID("attackAllBtn").classList.toggle("btn-inactive");

  	_function.toggleDisplay("modal", 'show');
  	_function.getEleByID("btnOkTiming").onclick = ()=>{
  		_function.toggleDisplay("modal", 'show');
  		let time = _function.getEleByID("selectTiming").value;

	  	let url = `/deDisAttack.json?time=${time}&` + `type=`; 
	  		
	  	url += (de_dis==='de')?"0":"1";
	  	url += isAll?"&all=1":"&all=0";

	  	fetch(url).then(res=>res.json())
	  	.then(data=>{
	  		if(data){
	  			let checkStatus = setInterval(()=>{
	  				fetch("/checkStatus.json").then(res=>res.json())
	  				.then(check=>{
	  					if(!check){
	  						clearInterval(checkStatus);
	  						_function.getEleByID("attack-station-btn").classList.toggle("btn-inactive");
	  						_function.getEleByID("attackAllBtn").classList.toggle("btn-inactive");
	  					}
	  				})
	  			}, 500)
	  		}
	  	})
	  }
  }

  function selectST(i) {
  	fetch("/selectStation.json?id="+i).then(res=>res.json())
  	.then(data=>{
  		if(data){
  			let checkStatus = setInterval(()=>{
  				fetch("/checkStatus.json").then(res=>res.json())
  				.then(check=>{
  					if(!check){
  						clearInterval(checkStatus);
  						if(_function.hasClass("selectSTId"+i, "btn-active")) numSelectST--;
  						else numSelectST++;

  						console.log(numSelectST);
							_function.getEleByID("selectSTId"+i).classList.toggle("btn-active");

							if(numSelectST == 0 || (numSelectST > 0 && !_function.hasClass("attack-station-btn", "show")))
								_function.toggleDisplay("attack-station-btn", 'show');
  					}
  				})
				}, 500)
  		}
  	})
  }

  function startScanST() {
  	_function.getEleByID("scanStationBtn").classList.toggle("btn-inactive");

  	_function.toggleDisplay("modal", 'show');

  	_function.getEleByID("btnOkTiming").onclick = ()=>{
  		_function.toggleDisplay("modal", 'show');
  		let time = _function.getEleByID("selectTiming").value;
	  	fetch("/startScanStation.json?time="+time).then(res=>{
	  		res.json().then(data=>{
	  			if(data){
	  				let checkStatus = setInterval(()=>{
		  				fetch("/checkStatus.json").then(res=>{
		  					res.json().then(check=>{
		  						if(!check){
		  							clearInterval(checkStatus);
		  							fetch("/requestSendScanStationResults.json").then(res=>{
		  								res.json().then(data=>{
		  									if(data){
		  										let checkStatus1 = setInterval(()=>{
		  											fetch("/checkStatus.json").then(res=>{
		  												res.json().then(check=>{
		  													console.log("checkStatus:", check);
		  													if(!check){
		  														clearInterval(checkStatus1);
		  														fetch("/sendScanStationResults.json").then(res=>{
		  															res.json().then(data=>{
		  																_function.getEleByID("bssidAPSL").innerHTML = data.bssid;
		  																let arData = data.station;
																			console.log("data:", arData);
																			let i = 0, htmlTable = "";
																			for(let data of arData){
																			htmlTable += `
																				<tr>
																					<td>${i+1}</td>
																					<td>${data}</td>
																					<td>
																					<button class="btn-main btn-selectST" id="selectSTId${i}" >Select</button>
																					</td>
																				</tr>
																			`;
																			i++;
																			}
																			document.querySelector('#table-scan-station table thead').innerHTML = `
																				<th>STT</th><th>STATION MAC</th><th>SELECT</th>
																			`;
																			document.querySelector('#table-scan-station table tbody').innerHTML = htmlTable;

		  															})
		  														})
		  													}
		  												})
		  											})
		  										}, 500)
		  									}
		  								})
		  							})
		  						}
		  					})
		  				})
	  				}, 500)
	  			}
	  		})
	  	})
  	}
  }

  function selectAP(i) {
  	fetch("/selectAP.json?id="+i).then(res=>{
  		res.json().then(data=>{
  			if(data){
  				let checkStatus = setInterval(()=>{
  					fetch("/checkStatus.json").then(res=>{
  						res.json().then(check=>{
  							if(!check){
  								clearInterval(checkStatus);
  								var arBtn = document.getElementsByClassName("btn-selectAP");
  								for(let i = 0; i < arBtn.length; i++){
  									arBtn[i].classList.toggle("btn-inactive"); 
  								}
  								_function.getEleByID("selectAPId"+i).classList.toggle("btn-inactive");
  								_function.getEleByID("selectAPId"+i).classList.toggle("btn-active");

  								_function.toggleDisplay("scan-station-btn", 'show');
  							}
  						})
  					})
  				}, 500)
  			}
  		})
  	})
  }

  function startScanAp() {

  	_function.getEleByID("scanAp-btn").classList.toggle("btn-inactive");

		let rs = fetch('/startScanAP.json');
		rs.then(res=>{
			res.json().then(data=>{
				if(data){
					console.log("startScanAP ", data);
					let checkStatus = setInterval(()=>{
						fetch('/checkStatus.json').then(res=>{
							res.json().then(data=>{
								console.log("checkStatus:", data);
								if(!data){
									clearInterval(checkStatus);
									console.log("START get result");
									fetch('/requestSendScanAPResults.json').then(res1=>{
										res1.json().then(data=>{
											console.log("requestSendScanAPResults:", data);
											if(data){
												let checkStatus1 = setInterval(()=>{
													fetch('/checkStatus.json').then(res=>{
														res.json().then(data=>{
															console.log("checkStatus1:", data);
															if(!data){
																clearInterval(checkStatus1);
																fetch('/sendScanAPResults.json').then(res=>{
																	res.json().then(data=>{
																		let arData = data.aps;
																		console.log("data:", arData);
																		let i = 0, htmlTable = "";
																			
																		for(let data of arData){
																			htmlTable += `
																				<tr>
																					<td><p>${i+1}</p></td>
																					<td><p>${data.n}</p></td>
																					<td><p>${data.b}</p></td>
																					<td><p>${data.c}</p></td>
																					<td><p>${data.r}</p></td>
																					<td><p>${data.e}</p></td>
																					<td><p>${data.h}</p></td>
																					<td>
																						<button class="btn-main btn-selectAP" id="selectAPId${i}" >Select</button>
																					</td>
																				</tr>
																			`;
																			i++;
																		}
																		document.querySelector('#tabel-scan-contain table thead').innerHTML = `
																			<tr><th>STT</th><th>ESSID</th><th>BSSID</th><th>CHANEL</th><th>RSSI</th><th>ENCRYPT</th><th>HIDEN</th><th>SELECT</th></tr>
																		`;
																		document.querySelector('#tabel-scan-contain table tbody').innerHTML = htmlTable;
																		

																	})
																})
															}
														})
													})
												}, 500)
											}
										})

									})
								}
							})
						})
					}, 500)
				}
			})
		})
	}

}( window.scan = window.scan || {}));


(function(monitor) {
    //Private Property
    var intervalGetData;
    var numCheck = 0;
    var time = 10;
    var timeDelay = 2000;
    var maxGoiTin = 40;
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

    	dramX = w/(time/2),
			dramY = h/(maxGoiTin/5);

    	//draw cot doc
    	ctx.beginPath();
    	ctx.strokeStyle = "white";
    	ctx.moveTo(dramX/2,dramY);
			ctx.lineTo(dramX/2,h-dramY);
			ctx.stroke();

			ctx.moveTo(dramX/2,dramY);
			ctx.lineTo(dramX/2 - dramX/4, dramY + dramX/4);
			ctx.stroke();

			ctx.moveTo(dramX/2,dramY);
			ctx.lineTo(dramX/2 + dramX/4, dramY + dramX/4);
			ctx.stroke();

			//draw doc ngang
			ctx.beginPath();
			ctx.strokeStyle = "white";
    	ctx.moveTo(dramX/2,h-dramY);
			ctx.lineTo(w - dramX/2, h-dramY);
			ctx.stroke();

			ctx.moveTo(w - dramX/2,h-dramY);
			ctx.lineTo(w - dramX/2 - dramX/4, h-dramY - dramX/4);
			ctx.stroke();

			ctx.moveTo(w - dramX/2,h-dramY);
			ctx.lineTo(w - dramX/2 - dramX/4, h-dramY + dramX/4);
			ctx.stroke();

			x1=x2=x3 = dramX/2;
			y1=y2=y3 = h-dramY;
    }

    monitor.drawChart = ()=>{
    	ctx.beginPath();
    	ctx.strokeStyle = "green";
    	ctx.moveTo(x1,y1);
			x1 = x1+(dramX*(time/2-1))/(time/2); y1 = value.bc*dramY/5;
			ctx.lineTo(x1, y1);
			ctx.stroke();

			ctx.beginPath();
			ctx.strokeStyle = "red";
			ctx.moveTo(x2,y2);
			x2 = x2+(dramX*(time/2-1))/(time/2); y2 = value.de==0?(h - dramY):(h - value.de*dramY/5);
			ctx.lineTo(x2, y2);
			ctx.stroke();

			ctx.beginPath();
			ctx.strokeStyle = "yellow";
			ctx.moveTo(x3,y3);
			x3 = x3+(dramX*(time/2-1))/(time/2); y3 = value.dis==0?(h - dramY):(h - value.dis*dramY/5);
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
	  		
	  		monitor.initChart();
		setInterval(()=>{
			value.bc = _function.randInt(10,15);
			value.de = _function.randInt(10,15);
			value.dis = _function.randInt(10,15);
			monitor.drawChart();
		}, 2000)

    		let url= `/startMonitorEnv.json?time=${time}&channel=1`;
    		fetch(url).then(res=>res.json())
    		.then(data=>{
    			if(data){
    				let checkStatus = setInterval(()=>{
    					fetch('/checkStatus.json').then(res=>res.json())
    					.then(check=>{
    						if(check){
    							numCheck++;
    							//monitor.getData();
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

// _function.getEleByID("scanAp-btn").onclick = scan.startScanAp;
// _function.getEleByID("scanStationBtn").onclick = scan.startScanST;

// _function.getEleByID("attackAllBtn").onclick = ()=>{scan.Attack('de', true);}
// _function.getEleByID("DeAttackBtn").onclick = ()=>{scan.Attack('de', false);}
// _function.getEleByID("DisAttackBtn").onclick = ()=>{scan.Attack('dis', false);}

document.addEventListener("click", (e)=>{
	var id = e.target.id;
	if(id.indexOf("selectAPId") != -1){
		scan.selectAP(parseInt(id.split("").reverse().join("")));
	}
	if(id.indexOf("selectSTId") != -1){
		scan.selectST(parseInt(id.split("").reverse().join("")));
	}
})

// (function( dude ) {
//     //Private Property

//     //Public Method
    
//     //Private Method

// }( window.dude = window.dude || {}));

