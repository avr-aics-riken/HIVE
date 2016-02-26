import React from "react"
import ReactDOM from "react-dom"
import Core from '../../Core'

import NodeView from "./NodeView.jsx"
import NodePlugView from "./NodePlugView.jsx"
import Property from "./Property"
import Store from "./Store.jsx"
import Action from "./Action.jsx"
import NodeListCreate from "./NodeListCreate.jsx"

var Dispatcher = require("flux").Dispatcher;

/**
 * 全てのノードを内包するビュー.
 */
export default class View extends React.Component {
	constructor(props) {
        super(props);

        this.state = {
            visible: false
        };
        this.listVisiblity = false;

		//var dispatcher =  new Dispatcher();
		this.nodeStore = new Store(this.props.action.dispatcher, this.props.store);
		this.nodeAction = new Action(this.props.action.dispatcher, this.nodeStore.getDispatchToken());
        this.dblClickEvent = this.dblClickEvent.bind(this);
        this.keyDownEvent = this.keyDownEvent.bind(this);
        this.componentDidMount = this.componentDidMount.bind(this);
        this.generator = this.generator.bind(this);
    }

    dblClickEvent(eve){
        this.listVisiblity = !this.listVisiblity;
        this.setState({visible: this.listVisiblity});
        if(this.listVisiblity){
            setTimeout((()=>{
                var e = ReactDOM.findDOMNode(this.refs.creator.refs.creator.refs.suggest.input);
                e.focus();
            }).bind(this), 100);
        }
    }

    keyDownEvent(eve){
        switch(eve.keyCode){
            case 27:
                this.listVisiblity = false;
                this.setState({visible: false});
                break;
            case 32:
                this.listVisiblity = true;
                this.setState({visible: true});
                setTimeout((()=>{
                    var e = ReactDOM.findDOMNode(this.refs.creator.refs.creator.refs.suggest.input);
                    e.focus();
                }).bind(this), 100);
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
        if(this.state.visible){
            return (
                <NodeListCreate
                    store={this.props.store}
                    action={this.props.action}
                    visibility={this.state.visible}
                    ref="creator"
                />
            );
        }
    }

	render () {
		return (<div style={{position:"absolute",width:"100%",height:"100%"}}>
					<NodeView
						store={this.props.store}
						action={this.props.action}
						nodeStore={this.nodeStore}
						nodeAction={this.nodeAction}
					/>
					<NodePlugView
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
