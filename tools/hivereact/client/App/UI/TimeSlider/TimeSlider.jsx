import React from "react";
import ReactDOM from "react-dom";
import Core from '../../Core';

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
                width: "100%",
                height: "100%"
            }
        };
    }

    render(){
        const style = this.styles();
        return (
            <div style={style.sliderArea}>
                time slider area
            </div>
        );
    }
}

