import React from "react";
import ReactDOM from "react-dom";
import Core from '../../Core';
import FileBrowser from "./FileBrowser.jsx"

/**
 * ファイルブラウザ
 */
export default class View extends React.Component {
	constructor(props) {
		super(props);
		this.store = this.props.store;
		this.action = this.props.action;
	}

	render () {
		return (<FileBrowser
					display={this.props.display}
					okFunc={this.props.okFunc}
					cancelFunc={this.props.cancelFunc}
					action={this.props.action}
					store={this.props.store}
				/>);
	}
}
