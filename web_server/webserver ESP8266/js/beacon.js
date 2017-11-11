(function( _beacon ) {
    //Private Property
    var isRamdom = false;
    var time = 10;
    var ssid = "";
    var url = "";
    //Public Method
    _beacon.init = () =>{
    	_function.getEleByID("randomATBC").onclick = ()=>{
    		isRamdom = true;
    		_function.getEleByID("ssid").disabled  = true;
    		_function.getEleByID("ssid").value = "";
    	}
    	_function.getEleByID("custumATBC").onclick = ()=>{
    		isRamdom = false;
    		_function.getEleByID("ssid").disabled  = false;
    	}
    }
    _beacon.attack = ()=>{
    	time = _function.getEleByID("timeAttackBc").value;
    	ssid = _function.getEleByID("ssid").value;

    	if((isRamdom && time != "") || (!isRamdom && time !="" && ssid != "" && ssid.indexOf(" ") == -1)){
    		url = `/beaconAttack.json?random=${ssid==''?'1':'0'}&time=${time}&ssid=${ssid}`;
    		console.log(url); 

    		_function.getEleByID("beacon-attack-btn").classList.toggle("btn-inactive");

    		fetch(url).then(res=>res.json())
    		.then(data=>{
    			if(data){
    				let checkStatus = setInterval(()=>{
                        fetch('/checkStatus.json').then(res=>res.json())
                        .then(check=>{
                            if(!check){
                                clearInterval(checkStatus);
                                _function.getEleByID("beacon-attack-btn").classList.toggle("btn-inactive");
                            }
                        }) 
                    }, 500)
    			}
    		})
    	} else{
    		alert("Input not valid, please input again. Thanks!");
    	}
    }
    //Private Method

}( window._beacon = window._beacon || {}));
_beacon.init();