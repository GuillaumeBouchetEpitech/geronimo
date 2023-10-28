class P{_textAreaElement;_lines;_maxLines;constructor(q){this._textAreaElement=q,this._textAreaElement.value="",this._lines=[],this._maxLines=30}log(...q){if(q.length==0)return;const D=Array.prototype.slice.call(q).join(" ");console.log(D),this._pushText(D)}error(...q){if(q.length==0)return;const D=Array.prototype.slice.call(q).join(" ");console.error(D),this._pushText(`[ERR] - ${D}`)}_pushText(q){if(this._lines.push(q),this._lines.length>this._maxLines)this._lines.splice(0,this._lines.length-this._maxLines);this._textAreaElement.value=`${this._lines.join("\n")}\n`,this._textAreaElement.scrollTop=this._textAreaElement.scrollHeight}get size(){return this._lines.length}peekLast(){if(this._lines.length>0)return this._lines[this._lines.length-1];return}popLast(){if(this._lines.length>0)this._lines.splice(this._lines.length-1,1)}}var G=P;var C=/[?&]+([^=&]+)=([^&]*)/gi,I=()=>{const q={};let D=null;while(D=C.exec(window.location.href)){const B=D[1],Q=D[2];q[B]=Q}return q};var M=(q)=>{return new Promise((D,B)=>{const Q=document.createElement("script");Q.src=q,Q.addEventListener("load",()=>D),Q.addEventListener("error",B),document.head.appendChild(Q)})};var W=(q)=>{if(!window.WebGL2RenderingContext)throw new Error("missing WebGL2 feature (unsupported)");const D={alpha:!1,antialias:!1,depth:!0,failIfMajorPerformanceCaveat:!1,powerPreference:"high-performance",premultipliedAlpha:!0,preserveDrawingBuffer:!0,stencil:!1},B=q.getContext("webgl2",D);if(!B)throw new Error("WebGL2 context failed (initialization)");return B};var b=()=>{return(document.pointerLockElement||document.mozPointerLockElement||document.webkitPointerLockElement)!==void 0};var w=()=>{return(()=>{try{if(typeof WebAssembly==="object"&&typeof WebAssembly.instantiate==="function"){const D=new WebAssembly.Module(Uint8Array.of(0,97,115,109,1,0,0,0));if(D instanceof WebAssembly.Module)return new WebAssembly.Instance(D)instanceof WebAssembly.Instance}}catch(D){}return!1})()};var A=()=>{return!!window.Worker};var F=()=>{return!!window.WebGL2RenderingContext};var T=()=>{const q=window.SharedArrayBuffer!==void 0;if(q){const B={initial:8,maximum:8,shared:!0};if(!(new WebAssembly.Memory(B).buffer instanceof SharedArrayBuffer))return!1}return q};class U{_canvas;_webglCtx;_width=800;_height=600;_isInitialized=!1;_isAborted=!1;_wasmApplicationUpdateFunc;_wasmApplicationRenderFunc;_onProgress;_onError;constructor(q,D,B){this._canvas=q,this._onProgress=D,this._onError=B}async initialize(q,D,B){if(this._width=q,this._height=D,!A())throw new Error("missing WebWorker feature (unsupported)");if(B.log("[JS][check] WebWorker feature => supported"),!w())throw new Error("missing WebAssembly feature (unsupported)");if(B.log("[JS][check] WebAssembly feature => supported"),T())B.log("[JS][check] multithreading => supported");else B.log("[JS][check] multithreading => NOT supported");if(!F())throw new Error("missing WebGL2 feature (unsupported)");B.log("[JS][check] WebGL2 feature => supported");const Q=()=>{if(this._isAborted=!0,this._onError)this._onError("Could not create a WebGL2 context.")};this._canvas.addEventListener("webglcontextcreationerror",Q,!1);const y=()=>{if(this._isAborted=!0,this._onError)this._onError("WebGL2 context lost. You will need to reload the page.")};this._canvas.addEventListener("webglcontextlost",y,!1);const N=(_)=>{_.preventDefault()};this._canvas.addEventListener("contextmenu",N,!1),this._webglCtx=W(this._canvas),B.log("[JS] WebGL2 context initialized");const X="wasm";return new Promise((_,j)=>{const Y={downloadingDataRegExp:/Downloading data\.\.\. \(([0-9]*)\/([0-9]*)\)/,lastProgressLevel:0,locateFile:(K)=>{return`${X}/${K}`},print:(K)=>{B.log(`[C++][out] ${K}`)},printErr:(K)=>{B.error(`[C++][err] ${K}`)},setStatus:(K)=>{if(!K)return;const z=Y.downloadingDataRegExp.exec(K);if(z){const R=parseFloat(z[1]),k=parseFloat(z[2]),$=Math.floor(R/k*100);if(Y.lastProgressLevel!==$)Y.lastProgressLevel=$,this._onProgress($)}else B.log(`[JS][wasm][status] ${K}`)},onRuntimeInitialized:()=>{B.log("[JS][wasm] loaded"),B.log("[JS][wasm] initialising");const K={startApplication:window.Module.cwrap("startApplication",void 0,["number","number","number","number"]),updateApplication:window.Module.cwrap("updateApplication",void 0,["number","number"]),renderApplication:window.Module.cwrap("renderApplication",void 0,[])};this._wasmApplicationUpdateFunc=K.updateApplication,this._wasmApplicationRenderFunc=K.renderApplication,K.startApplication(this._width,this._height),this._isInitialized=!0,B.log("[JS][wasm] initialized"),_()},canvas:this._canvas,preinitializedWebGLContext:this._webglCtx,noInitialRun:!0,noExitRuntime:!0};window.Module=Y,B.log("[JS][wasm] loading"),M(`./${X}/index.js`).catch(j)})}update(q){if(!this._isInitialized||this._isAborted)return;if(this._wasmApplicationUpdateFunc){const D=b()?1:0;this._wasmApplicationUpdateFunc(q,D)}}render(){if(!this._isInitialized||this._isAborted)return;if(this._wasmApplicationRenderFunc)this._wasmApplicationRenderFunc()}abort(){if(!this._isInitialized||this._isAborted)return;this._isAborted=!0;const q=window.Module;if(q)q.setStatus=(D)=>{if(D)console.error(`[JS][wasm][aborted] ${D}`)}}}var O=(q)=>{const D=document.querySelector(q);if(!D)throw new Error(`DOM elements not found, id=${q}`);return D},v=async()=>{let q=!0;const D=O("#loggerOutput"),B=new G(D);B.log("[JS] page loaded");const Q=(J)=>{q=!1,B.error(`[JS] exception, event=${J}`)};window.addEventListener("error",Q);const y=O("#errorText"),N=O("#renderArea"),X=O("#canvas"),_=(J)=>{if(J.style.display!=="none")J.style.display="none"},j=(J)=>{if(J.style.display!=="block")J.style.display="block"},Y=(J)=>{y.innerHTML=J,_(X),j(y)},K=()=>{_(y),j(X)},z=(J,Z)=>{N.style.maxWidth=X.style.maxWidth=`${J}px`,N.style.maxHeight=X.style.maxHeight=`${Z}px`,X.width=J,X.height=Z},R=(J)=>{Q(J),Y(`fatal error, event=${J}`)};window.removeEventListener("error",Q),window.addEventListener("error",R);const k=(J,Z)=>J?parseInt(J,10):Z,$=I(),H={width:k($.width,800),height:k($.height,600)};z(H.width,H.height);const S=new U(X,(J)=>{const Z=`Loading wasm [${J}%]`;if(B.size>0&&B.peekLast().indexOf("Loading wasm [")>=0)B.popLast();B.log(`[JS] ${Z}`),Y(Z)},Y);try{await S.initialize(H.width,H.height,B),K()}catch(J){B.error(`[JS] dependencies check failed: message="${J.message}"`),Y(["this browser isn't compatible","error message:",`=> ${J.message}`].join("<br>"))}let V=Date.now();const f=()=>{if(q)requestAnimationFrame(f);const J=Date.now(),Z=J-V;V=J,S.update(Z),S.render()};f()};window.addEventListener("load",v);
