import React from "react"
import ReactDOM from "react-dom"
import Menu from "./Menu.jsx"

export default class View extends React.Component {
    constructor(props) {
        super(props);
    }

    render(){
        return (
            <Menu store={this.props.store} action={this.props.action} nodeSystem={this.props.nodeSystem} />
        );
    }
}
