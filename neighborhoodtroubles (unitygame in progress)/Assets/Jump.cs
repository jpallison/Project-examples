using UnityEngine;
using System.Collections;

public class Jump : MonoBehaviour {
	public float jumpHeight;
	public float multiplier;
	public bool canJump = true;

	void OnCollisionEnter2D(Collision2D coll)
	{
		if (coll.gameObject.tag == "ground") {
			Debug.Log ("colliders are colliding");
			canJump = true;		
		}
	}

	// Update is called once per frame
	void Update () {
		if (Input.GetKeyDown (KeyCode.Space) && canJump) {
			rigidbody2D.AddForce (Vector2.up * jumpHeight);
			canJump = false;
		}
	}
}
