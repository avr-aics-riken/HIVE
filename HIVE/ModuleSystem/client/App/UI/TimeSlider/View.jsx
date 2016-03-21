import React from "react";
import ReactDOM from "react-dom";
import TimeSlider from "./TimeSlider.jsx";

// var Dispatcher = require("flux").Dispatcher;

export default class View extends React.Component {
    constructor(props) {
        super(props);
        // var dispatcher = new Dispatcher();
        // this.menuStore = new MenuStore(dispatcher, this.props.store, this.props.layoutType);
        // this.menuAction = new MenuAction(dispatcher, this.menuStore.getDispatchToken());
    }

    render(){
        return (
            <TimeSlider
                store={this.props.store}
                action={this.props.action}
            />
        );
    }
}
