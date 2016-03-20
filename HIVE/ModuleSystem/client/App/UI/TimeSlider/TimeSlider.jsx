import React from "react";
import ReactDOM from "react-dom";
import Core from '../../Core';
import UMTimeline from './UMTimeline.jsx';

export default class TimeSlider extends React.Component {
    constructor(props) {
        super(props);

        this.state = {
        };

        this.store = this.props.store;
        this.action = this.props.action;
        this.styles = this.styles.bind(this);
        this.onClick = this.onClick.bind(this);
    }


    onClick(eve){
    }

    styles() {
        return {
            sliderArea: {
                backgroundColor: "#101018",
                color: "white",
				position : "absolute",
                width: "100%",
                height: "100%",
                overflow: "hidden"
            }
        };
    }

    render(){
        const style = this.styles();
        return (
            <div style={style.sliderArea}>
                <UMTimeline action={this.action} store={this.store} />
            </div>
        );
    }
}
