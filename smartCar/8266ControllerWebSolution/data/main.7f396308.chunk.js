(this.webpackJsonpcarauto=this.webpackJsonpcarauto||[]).push([[0],{12:function(t,e,o){},17:function(t,e,o){},18:function(t,e,o){"use strict";o.r(e);var n=o(1),l=o.n(n),r=o(3),i=o.n(r),c=(o(12),o(2)),a=function(){return Object(c.b)()},s=(c.c,o(4)),d=null;var h=Object(s.b)({name:"directionController",initialState:{controllerData:{left:{x:0,y:0,xGap:0,yGap:0,moving:!1,isInit:!1},right:{x:0,y:0,xGap:0,yGap:0,moving:!1,isInit:!1}},controllerRateConfig:{left_UD:100,left_LR:100,right_UD:100,right_LR:100,A:100,B:100},min:0,max:300,switchA:0,switchB:0,controllerSwith:!1,socketServer:"ws://192.168.5.1:81",socketServerObj:null},reducers:{setConfigMaxMin:function(t,e){for(var o in t.min=e.payload.x,t.max=e.payload.y,console.log("max:"+t.max+":min:"+t.min),t.controllerData)"left"===o&&(console.log("xxx"),console.log(t.controllerData[o]))},setDirection:function(t,e){t.controllerData[e.payload.controllerId]?(t.controllerData[e.payload.controllerId].x=e.payload.x,t.controllerData[e.payload.controllerId].y=e.payload.y,t.controllerData[e.payload.controllerId].xGap=e.payload.xGap,t.controllerData[e.payload.controllerId].yGap=e.payload.yGap,t.controllerData[e.payload.controllerId].moving=e.payload.moving,t.controllerData[e.payload.controllerId].isInit=e.payload.isInit):t.controllerData[e.payload.controllerId]={x:e.payload.x,y:e.payload.y,xGap:e.payload.xGap,yGap:e.payload.yGap,moving:e.payload.moving,isInit:!0}},setIsInit:function(t,e){t.controllerData[e.payload.controllerId].isInit=e.payload.isInit},sendData:function(t){!function(t){var e=t.controllerData.left.x,o=t.controllerData.left.y,n=t.controllerData.right.x,l=t.controllerData.right.y,r=t.switchA,i=t.switchB;t.controllerRateConfig.LEFT_UD<100&&(o=(t.controllerData.left.y-t.max/2)*t.controllerRateConfig.LEFT_UD/100+t.max/2),t.controllerRateConfig.left_LR<100&&(e=(t.controllerData.left.x-t.max/2)*t.controllerRateConfig.left_LR/100+t.max/2),t.controllerRateConfig.right_UD<100&&(l=(t.controllerData.right.y-t.max/2)*t.controllerRateConfig.right_UD/100+t.max/2),t.controllerRateConfig.right_LR<100&&(n=(t.controllerData.right.x-t.max/2)*t.controllerRateConfig.right_LR/100+t.max/2),t.controllerRateConfig.A<100&&(r=(t.switchA-t.max/2)*t.controllerRateConfig.A/100+t.max/2),t.controllerRateConfig.A<100&&(i=(t.switchB-t.max/2)*t.controllerRateConfig.B/100+t.max/2);var c=function(t){return Math.round(1e3*t)/1e3},a=-(l/t.max*250-125),s=-(n/t.max*250-125),h=Math.abs(s)*(s/125),u=a*(a/125),x=h,f=-h;if(console.log("1:"+x+":"+f),console.log(u),f+=u,x+=u,a>0){var g=f=-f;f=x=-x,x=g}f=125+f,x=125+x,console.log("2:"+x+":"+f);var b={command:"setController",clientType:"controller",data:{controllerData:{left_UD:c((t.max-o)/t.max*250),left_LR:c(e/t.max*250),right_UD:x,right_LR:250-f,A:r,B:i}}};try{d.send(JSON.stringify(b))}catch(j){}}(t)},togglePowerStatus:function(t){if(t.controllerSwith=!t.controllerSwith,t.controllerSwith)null==d&&((d=new WebSocket(t.socketServer)).onopen=function(t){console.log("Connection open ..."),d.send(JSON.stringify({command:"identify",data:{name:"controller"}}))},d.onclose=function(t){console.log("Connection closed.")});else{try{d.close()}catch(e){}d=null}},increaseSwithA:function(t){t.switchA=t.switchA+1},increaseSwithB:function(t){t.switchB=t.switchB+1},decreaseSwithA:function(t){0!==t.switchA&&(t.switchA=t.switchA-1)},decreaseSwithB:function(t){0!==t.switchB&&(t.switchB=t.switchB-1)},changeSocketServer:function(t,e){t.socketServer=e.payload.socketServer},setConfigRateData:function(t,e){t.controllerRateConfig=e.payload}},extraReducers:function(t){}}),u=h.actions,x=u.setIsInit,f=u.setConfigRateData,g=u.setConfigMaxMin,b=u.changeSocketServer,j=u.setDirection,v=u.sendData,y=u.togglePowerStatus,p=u.increaseSwithA,m=u.increaseSwithB,I=u.decreaseSwithA,C=u.decreaseSwithB,O=function(t){return{directionController:t.directionController.controllerData}},w=function(t){return t.directionController.controllerSwith},D=function(t){return t.directionController.switchA},R=function(t){return t.directionController.switchB},S=function(t){return t.directionController.socketServer},G=function(t){return t.directionController.controllerRateConfig},k=h.reducer,A=o(0);function N(t,e){for(var o=t.touches,n=0;n<o.length;n++){if(e===o[n].target.getAttribute("data-controller_id"))return{x:o[n].clientX,y:o[n].clientY,controllerId:e}}}var _=function(t){var e=document.body.clientHeight/2,o=e/3,n=(e-o)/2,l=n,r=n,i=a(),s=Object(c.c)(O),d=s.directionController[t.controllerId].moving,h=s.directionController[t.controllerId].isInit;d=null!=d&&d;var u=null!=s.directionController[t.controllerId]?s.directionController[t.controllerId]:{x:n,y:2*n};h||i(j({x:(u={x:n,y:2*n}).x,y:u.y,controllerId:t.controllerId,xGap:0,yGap:0,moving:!1,isInit:!0}));var x=u.y,f=u.x;return Object(A.jsxs)("div",{className:"directionController",style:{width:e,height:e,borderRadius:e/2},children:[Object(A.jsx)("div",{className:"controllerBackground"}),Object(A.jsx)("div",{className:"controller","data-controller_id":t.controllerId,style:{width:o-2,height:o-2,borderRadius:o/2,top:x,left:f},onTouchStart:function(e){var o=N(e,t.controllerId),l=o.x,r=o.y;i(j({x:n,y:x,controllerId:t.controllerId,xGap:l-n,yGap:r-x,moving:!0,isInit:!0}))},onTouchEnd:function(e){i(j({x:n,y:x,controllerId:t.controllerId,xGap:0,yGap:0,moving:!1,isInit:!0}))},onTouchMove:function(e){var o=N(e,t.controllerId),c=s.directionController[t.controllerId].xGap,a=s.directionController[t.controllerId].yGap,d=s.directionController[t.controllerId].moving;l=o.x-c,r=o.y-a;var h=l-n,u=r-n;h=Math.abs(h),u=Math.abs(u);var x=Math.sqrt(h*h+u*u);if(x<=n)i(j({x:l,y:r,controllerId:t.controllerId,xGap:c,yGap:a,moving:d,isInit:!0}));else{var f=x/n,g=h/f,b=u/f;l>n?g+=n:g=-g+n,r>n?b+=n:b=-b+n,i(j({x:g,y:b,controllerId:t.controllerId,xGap:c,yGap:a,moving:d,isInit:!0}))}}})]})};function B(t,e){for(var o=t.touches,n=0;n<o.length;n++){if(e===o[n].target.getAttribute("data-controller_id"))return{x:o[n].clientX,y:o[n].clientY,controllerId:e}}}var L,M=function(t){var e=document.body.clientHeight/2,o=e/3,n=(e-o)/2,l=n,r=n,i=a(),s=Object(c.c)(O);return Object(A.jsxs)("div",{className:"directionController",style:{width:e,height:e,borderRadius:e/2},children:[Object(A.jsx)("div",{className:"controllerBackground"}),Object(A.jsx)("div",{className:"controller","data-controller_id":t.controllerId,style:{width:o-2,height:o-2,borderRadius:o/2,top:null!=s.directionController[t.controllerId].moving&&s.directionController[t.controllerId].moving&&s.directionController[t.controllerId]?s.directionController[t.controllerId].y:n,left:null!=s.directionController[t.controllerId].moving&&s.directionController[t.controllerId].moving&&s.directionController[t.controllerId]?s.directionController[t.controllerId].x:n},onTouchStart:function(e){var o=B(e,t.controllerId),l=o.x,r=o.y;i(j({x:n,y:n,controllerId:t.controllerId,xGap:l-n,yGap:r-n,moving:!0,isInit:!0}))},onTouchEnd:function(e){i(j({x:n,y:n,controllerId:t.controllerId,xGap:0,yGap:0,moving:!1,isInit:!0}))},onTouchMove:function(e){var o=B(e,t.controllerId),c=s.directionController[t.controllerId].xGap,a=s.directionController[t.controllerId].yGap,d=s.directionController[t.controllerId].moving;l=o.x-c,r=o.y-a;var h=l-n,u=r-n;h=Math.abs(h),u=Math.abs(u);var x=Math.sqrt(h*h+u*u);if(x<=n)i(j({x:l,y:r,controllerId:t.controllerId,xGap:c,yGap:a,moving:d,isInit:!0}));else{var f=x/n,g=h/f,b=u/f;l>n?g+=n:g=-g+n,r>n?b+=n:b=-b+n,i(j({x:g,y:b,controllerId:t.controllerId,xGap:c,yGap:a,moving:d,isInit:!0}))}}})]})};o(17);var U=function(){var t=a(),e=Object(c.c)(w),o=Object(c.c)(D),n=Object(c.c)(R),l=Object(c.c)(S),r=Object(c.c)(G),i=function(){var t=document.querySelectorAll(".rateConfig input"),e={};for(var o in t){var n=t[o];if(n&&n.getAttribute){var l=n.getAttribute("name"),i=n.value,c=parseFloat(i);e[l]=c>0&&c<=100||""===i?i:r[l]}}return e};return!1===e?L&&(clearInterval(L),L=null):null==L&&(L=setInterval((function(){t(v())}),200)),Object(A.jsxs)("div",{className:"App",children:[Object(A.jsx)("div",{style:{left:"calc(100vw / 2 - 25px)",top:30,position:"absolute",display:"inline-block"},children:Object(A.jsx)("div",{className:"power"+(e?" on":" off"),onClick:function(e){var o=document.body.clientHeight/2,n=o/3;t(y()),t(g({x:0,y:o-n,controllerId:null,xGap:0,yGap:0,moving:!1,isInit:!1})),t(x({controllerId:"left",isInit:!1}))},children:e?"ON":"OFF"})}),Object(A.jsx)("div",{style:{left:"calc(100vw / 2 - 100px)",top:80,position:"absolute",display:e?"none":"inline-block"},children:Object(A.jsx)("input",{onChange:function(e){t(b({socketServer:e.currentTarget.value}))},style:{width:200,border:"1px solid black",height:30,fontSize:"1.2rem",textAlign:"center"},value:l})}),Object(A.jsxs)("div",{className:"rateConfig",style:{display:e?"none":"flex"},children:[Object(A.jsxs)("div",{className:"rateConfigItem",children:[Object(A.jsx)("label",{children:"Left UP-Down"}),Object(A.jsx)("input",{name:"left_UD",value:r.left_UD,onChange:function(){t(f(i()))}}),"\xa0%"]}),Object(A.jsxs)("div",{className:"rateConfigItem",children:[Object(A.jsx)("label",{children:"Right UP-Down"}),Object(A.jsx)("input",{name:"right_UD",value:r.right_UD,onChange:function(){t(f(i()))}}),"\xa0%"]}),Object(A.jsxs)("div",{className:"rateConfigItem",children:[Object(A.jsx)("label",{children:"Left Left-Right"}),Object(A.jsx)("input",{name:"left_LR",value:r.left_LR,onChange:function(){t(f(i()))}}),"\xa0%"]}),Object(A.jsxs)("div",{className:"rateConfigItem",children:[Object(A.jsx)("label",{children:"Right Left-Right"}),Object(A.jsx)("input",{name:"right_LR",value:r.right_LR,onChange:function(){t(f(i()))}}),"\xa0%"]}),Object(A.jsxs)("div",{className:"rateConfigItem",children:[Object(A.jsx)("label",{children:"Switch A"}),Object(A.jsx)("input",{name:"A",value:r.A,onChange:function(){t(f(i()))}}),"\xa0%"]}),Object(A.jsxs)("div",{className:"rateConfigItem",children:[Object(A.jsx)("label",{children:"Switch B"}),Object(A.jsx)("input",{name:"B",value:r.B,onChange:function(){t(f(i()))}}),"\xa0%"]})]}),Object(A.jsxs)("div",{style:{display:e?"block":"none"},children:[Object(A.jsx)("div",{style:{left:50,top:30,position:"absolute",display:"inline-block"},children:Object(A.jsx)(_,{controllerId:"left"})}),Object(A.jsx)("div",{style:{right:50,top:30,position:"absolute",display:"inline-block"},children:Object(A.jsx)(M,{controllerId:"right"})}),Object(A.jsx)("div",{style:{left:80,bottom:15,position:"absolute",display:"inline-block"},children:Object(A.jsxs)("div",{className:"swither",children:[Object(A.jsx)("div",{className:"data",children:o}),Object(A.jsxs)("div",{className:"buttons",children:[Object(A.jsx)("div",{className:"switcherButton",style:{height:"100%",width:"calc(100% / 2)"},onClick:function(){t(I())},children:" - "}),Object(A.jsx)("div",{className:"switcherButton",style:{height:"100%",width:"calc(100% / 2)",borderLeft:"1px solid black"},onClick:function(){t(p())},children:" + "})]})]})}),Object(A.jsx)("div",{style:{right:80,bottom:15,position:"absolute",display:"inline-block"},children:Object(A.jsxs)("div",{className:"swither",children:[Object(A.jsx)("div",{className:"data",children:n}),Object(A.jsxs)("div",{className:"buttons",children:[Object(A.jsx)("div",{className:"switcherButton",style:{height:"100%",width:"calc(100% / 2)"},onClick:function(){t(C())},children:" - "}),Object(A.jsx)("div",{className:"switcherButton",style:{height:"100%",width:"calc(100% / 2)",borderLeft:"1px solid black"},onClick:function(){t(m())},children:" + "})]})]})})]})]})},T=Object(s.a)({reducer:{direction:k,directionController:k}});i.a.render(Object(A.jsx)(l.a.StrictMode,{children:Object(A.jsx)(c.a,{store:T,children:Object(A.jsx)(U,{})})}),document.getElementById("root"))}},[[18,1,2]]]);
//# sourceMappingURL=main.7f396308.chunk.js.map