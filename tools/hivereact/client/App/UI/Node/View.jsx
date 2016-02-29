import React from "react";
import ReactDOM from "react-dom";
import Core from '../../Core';
;
import NodeView from "./NodeView.jsx";
import NodePlugView from "./NodePlugView.jsx";
import Property from "./Property";
import Store from "./Store.jsx";
import Action from "./Action.jsx";
import NodeListCreate from "./NodeListCreate.jsx";

var Dispatcher = require("flux").Dispatcher;

/**
 * 全てのノードを内包するビュー.
 */
export default class View extends React.Component {
	constructor(props) {
        super(props);

        this.store = this.props.store;
        this.action = this.props.action;
        this.state = {
            listVisible: false,
            listPos: []
        };
        this.listVisiblity = false;
        this.focusTarget = null;

		//var dispatcher =  new Dispatcher();
		this.nodeStore = new Store(this.props.action.dispatcher, this.props.store);
		this.nodeAction = new Action(this.props.action.dispatcher, this.nodeStore.getDispatchToken());
        this.setFocusTarget = this.setFocusTarget.bind(this);
        this.dblClickEvent = this.dblClickEvent.bind(this);
        this.keyDownEvent = this.keyDownEvent.bind(this);
        this.componentDidMount = this.componentDidMount.bind(this);
        this.generator = this.generator.bind(this);
    }

    // この関数をフォーカスしたい子要素に渡して呼んでもらう
    setFocusTarget(element){
        this.focusTarget = element;
    }

    // 現状は NodeView 内にある SVG Element から呼ばれる
    dblClickEvent(eve){
        this.listVisiblity = !this.listVisiblity;
        this.setState({
            listVisible: this.listVisiblity,
            listPos: [eve.layerX, eve.layerY]
        });
        if(this.listVisiblity){
            setTimeout((()=>{
                var e = ReactDOM.findDOMNode(this.focusTarget.refs.suggest.input);
                e.focus();
            }).bind(this), 50);
        }
    }

    // キーダウンイベントのターゲットは Window
    keyDownEvent(eve){
        switch(eve.keyCode){
            case 27:
                this.listVisiblity = false;
                this.setState({listVisible: false});
                break;
            case 32:
                this.setState({
                    listVisible: true
                });
                setTimeout((()=>{
                    let el, x, y, w, h;
                    el = ReactDOM.findDOMNode(this.focusTarget);
                    el = el.parentNode.parentNode; // temp
                    w = el.clientWidth;
                    h = el.clientHeight;
                    x = w / 2 - 100; // temp
                    y = h / 2 - 150; // temp
                    this.setState({listPos: [x, y]});
                    var e = ReactDOM.findDOMNode(this.focusTarget.refs.suggest.input);
                    e.focus();
                }).bind(this), 50);
                break;
            default:
                break;
        }
    }

    componentDidMount(){
        var e = this.refs.plugView.refs.svg;
        e.addEventListener('dblclick', this.dblClickEvent.bind(this), true);
        window.addEventListener('keydown', this.keyDownEvent.bind(this));
    }

    componentWillUnmount(){
        window.removeEventListener('keydown', this.keyDownEvent.bind(this));
    }

    generator(){
        if(this.state.listVisible){
            return (
                <NodeListCreate
                    store={this.props.store}
                    action={this.props.action}
                    visibility={this.state.listVisible}
                    position={this.state.listPos}
                    focusFunction={this.setFocusTarget.bind(this)}
                    ref="creator"
                />
            );
        }
    }


	render () {
		return (<div style={{position:"absolute",width:"100%",height:"100%",overflow:"hidden"}}>
					<NodeView
						store={this.props.store}
						action={this.props.action}
						nodeStore={this.nodeStore}
						nodeAction={this.nodeAction}
					/>
					<NodePlugView
						style={{zIndex:"1"}}
						store={this.props.store}
						action={this.props.action}
						nodeStore={this.nodeStore}
						nodeAction={this.nodeAction}
                        ref="plugView"
					/>
					<Property.View
						store={this.props.store}
						action={this.props.action}
					/>
                    {this.generator.bind(this)()}
				</div>);
	}
}
