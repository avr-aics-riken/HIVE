import React from 'react'

class Time extends React.Component {
	constructor(props) {
		super(props);
		this.frameChanged = this.frameChanged.bind(this);
		
		this.action = props.action;
		this.store = props.store;
		
		setTimeout( ()=> {
			this.action.changePanelVisible(props.node.varname, false);
		}, 0);
	}

	frameChanged(err, frame) {
		// 特殊対応
		this.props.store.actionExecuter.changeNodeInput({
			inputInfo : {
				varname : this.props.node.varname,
				input : {
					"value" : frame 
				}
			}
		});
	}

	componentDidMount() {
		this.store.on("change_frame", this.frameChanged);
	}
	
	componentWillUnmount() {
		this.store.off("change_frame", this.frameChanged);
	}
	
	render() {
		return (<div></div>);
	}
}

module.exports = Time;
